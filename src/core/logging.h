// Copyright 2026 LingmoOS Contributors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LINGMO_CORE_LOGGING_H_
#define LINGMO_CORE_LOGGING_H_

#include <string_view>

#include "base/logging.h"

namespace Lingmo::Core {

// Written to with CF_DEBUG(cfInput) and friends.
using LogCategory = base::LogCategory;

// Applies `rules` (base::logging::setRules syntax) to every category.
void setLoggingRules(std::string_view rules);

} // namespace Lingmo::Core

// Startup and operational messages are on; routine window and input
// diagnostics need a debug rule (--log-rules).
extern const Lingmo::Core::LogCategory cfWayland;
extern const Lingmo::Core::LogCategory cfSession;
extern const Lingmo::Core::LogCategory cfDrm;
extern const Lingmo::Core::LogCategory cfRender;
extern const Lingmo::Core::LogCategory cfScene;
extern const Lingmo::Core::LogCategory cfCompositor;
extern const Lingmo::Core::LogCategory cfInput;
// Opt-in button/capture transitions; no pointer-motion or keyboard text logging.
extern const Lingmo::Core::LogCategory cfInputTransitions;
extern const Lingmo::Core::LogCategory cfCursorTransitions;

#endif // LINGMO_CORE_LOGGING_H_
