// Copyright 2026 LingmoOS Contributors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "shell/dock/dock_magnification.h"

#include <algorithm>
#include <cmath>

namespace Lingmo::Shell {

DockMagnification::DockMagnification(ui::Window *window)
    : FrameClient(window)
{
}

DockMagnification::~DockMagnification() = default;

void DockMagnification::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;
    m_enabled = enabled;
    m_placements = computeTargets();
    stopTicking();
    m_lastFrameNs = 0;
    if (onChanged)
        onChanged();
}

void DockMagnification::setFactor(double factor)
{
    factor = std::clamp(factor, 1.0, 2.0);
    if (m_factor == factor)
        return;
    m_factor = factor;
    if (isTicking())
        return;
    // A running transition picks the new factor up on its next frame.
    if (m_pointer) {
        m_lastFrameNs = 0;
        startTicking();
    } else {
        m_placements = computeTargets();
    }
}

void DockMagnification::setPointer(std::optional<double> along)
{
    if (m_pointer == along)
        return;
    m_pointer = along;
    if (!m_pointer) {
        // On the next frame the placements start coming back to rest.
        if (!isTicking())
            startTicking();
        return;
    }
    if (!isTicking()) {
        m_lastFrameNs = 0;
        startTicking();
    }
}

void DockMagnification::setItems(std::vector<Item> items, double pitch)
{
    const bool moved = items != m_items || std::max(1.0, pitch) != m_pitch;
    m_items = std::move(items);
    m_pitch = std::max(1.0, pitch);
    resizePlacements(computeTargets());
    // The row moved under a pointer that did not: the placements follow it.
    if (moved && m_pointer && m_enabled && m_factor > 1.0 && !isTicking()) {
        m_lastFrameNs = 0;
        startTicking();
    }
}

void DockMagnification::resizePlacements(const std::vector<Placement> &target)
{
    std::vector<Placement> next;
    next.reserve(m_items.size());
    const size_t kept = std::min(m_items.size(), m_placements.size());
    for (size_t i = 0; i < m_items.size(); ++i) {
        // The items there were before keep their place in the transition; one
        // that just arrived is drawn where it belongs at once.
        next.push_back(i < kept ? m_placements[i] : target[i]);
    }
    m_placements = std::move(next);
}

std::vector<DockMagnification::Placement> DockMagnification::computeTargets() const
{
    std::vector<Placement> result(m_items.size());
    if (!m_enabled || m_factor <= 1.0 || !m_pointer || m_items.empty())
        return result;

    const double pointer = *m_pointer;
    const double pitch = std::max(1.0, m_pitch);

    // A quadratic bell over the reach: 1 over the item, nothing kReachSlots
    // away, smooth at both ends.
    std::vector<double> scales(m_items.size());
    for (size_t i = 0; i < m_items.size(); ++i) {
        const double distance = (pointer - m_items[i].centre) / pitch / kReachSlots;
        const double weight = std::abs(distance) >= 1.0 ? 0.0 : 1.0 - distance * distance;
        scales[i] = 1.0 + (m_factor - 1.0) * weight * weight;
    }

    // The row with the scaled widths and the same gaps: where every item's
    // centre lands, before the row is moved back under the pointer.
    std::vector<double> centres(m_items.size());
    centres.front() = m_items.front().centre;
    for (size_t i = 1; i < m_items.size(); ++i) {
        centres[i] = centres[i - 1] + m_items[i - 1].width * scales[i - 1] / 2 + m_items[i].gap
                + m_items[i].width * scales[i] / 2;
    }

    // Moving the row so that the place under the pointer stays there: the
    // pointer's place between two resting centres maps onto the same place
    // between their scaled ones. Beyond the ends, and with one item, the row
    // keeps its place and only grows.
    double offset = 0;
    if (m_items.size() > 1 && pointer > m_items.front().centre && pointer < m_items.back().centre) {
        for (size_t i = 0; i + 1 < m_items.size(); ++i) {
            const double from = m_items[i].centre;
            const double to = m_items[i + 1].centre;
            if (pointer < from || pointer > to)
                continue;
            const double span = to - from;
            const double t = span > 0 ? (pointer - from) / span : 0.0;
            const double here = centres[i] + (centres[i + 1] - centres[i]) * t;
            offset = pointer - here;
            break;
        }
    }

    for (size_t i = 0; i < m_items.size(); ++i) {
        result[i].scale = scales[i];
        result[i].shift = centres[i] + offset - m_items[i].centre;
    }
    return result;
}

std::vector<DockMagnification::Placement> DockMagnification::targets() const
{
    return computeTargets();
}

void DockMagnification::tick(uint64_t frameTimeNs)
{
    const double nowMs = double(frameTimeNs) / 1e6;
    double elapsed = m_lastFrameNs ? nowMs - double(m_lastFrameNs) / 1e6 : kFrameIntervalMs;
    elapsed = std::clamp(elapsed, 1.0, 100.0);
    m_lastFrameNs = frameTimeNs;

    const std::vector<Placement> target = computeTargets();
    if (target.size() != m_placements.size())
        resizePlacements(target);

    // Following the pointer is quicker than letting go of it, as on macOS.
    const double tau = m_pointer ? kFollowTau : kReturnTau;
    const double approach = 1.0 - std::exp(-elapsed / (tau * 1000.0));
    bool changed = false;
    bool moving = false;
    for (size_t i = 0; i < m_placements.size(); ++i) {
        Placement &place = m_placements[i];
        const auto move = [approach, &moving](double &value, double to) {
            const double difference = to - value;
            if (std::abs(difference) <= kSettle) {
                value = to;
                return;
            }
            moving = true;
            value += difference * approach;
        };
        const double before = place.scale;
        const double shiftedBefore = place.shift;
        move(place.scale, target[i].scale);
        move(place.shift, target[i].shift);
        changed = changed || place.scale != before || place.shift != shiftedBefore;
    }

    if (!moving) {
        stopTicking();
        m_lastFrameNs = 0;
    }
    if (changed && onChanged)
        onChanged();
}

} // namespace Lingmo::Shell
