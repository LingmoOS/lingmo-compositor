// Copyright 2026 LingmoOS Contributors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/logging.h"

namespace Lingmo::Core {

void setLoggingRules(std::string_view rules)
{
    base::logging::setRules(rules);
}

} // namespace Lingmo::Core

const Lingmo::Core::LogCategory cfWayland("lingmo.wayland");
const Lingmo::Core::LogCategory cfSession("lingmo.session");
const Lingmo::Core::LogCategory cfDrm("lingmo.drm");
const Lingmo::Core::LogCategory cfRender("lingmo.render");
const Lingmo::Core::LogCategory cfScene("lingmo.scene");
const Lingmo::Core::LogCategory cfCompositor("lingmo.compositor");
const Lingmo::Core::LogCategory cfInput("lingmo.input");
const Lingmo::Core::LogCategory cfInputTransitions("lingmo.input.transitions");
const Lingmo::Core::LogCategory cfCursorTransitions("lingmo.cursor.transitions");
