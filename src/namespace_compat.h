// SPDX-FileCopyrightText: 2026 LingmoOS Contributors
// SPDX-License-Identifier: GPL-3.0-or-later

// The compositor's sources refer to its own components through lower-case
// names (scene::, wm::, display::, ...) while the components live in
// Lingmo::Scene, Lingmo::WM, Lingmo::Display, ... Because the sources are
// mostly inside Lingmo::<Component>, a lower-case name would first resolve to
// a nested namespace of Lingmo; these aliases give it a target at global
// scope. Included in every translation unit by the project's compile options.
#pragma once

#ifdef __cplusplus

namespace accelerators = Lingmo::Accelerators;
namespace auth = Lingmo::Auth;
namespace backend = Lingmo::Backend;
namespace clipboard = Lingmo::Clipboard;
namespace compositor = Lingmo::Compositor;
namespace computer_control = Lingmo::ComputerControl;
namespace core = Lingmo::Core;
namespace cursor = Lingmo::Cursor;
namespace decoration = Lingmo::Decoration;
namespace display = Lingmo::Display;
namespace scene = Lingmo::Scene;
namespace session = Lingmo::Session;
namespace shell = Lingmo::Shell;
namespace switcher = Lingmo::Switcher;
namespace ui = Lingmo::UI;
namespace wm = Lingmo::WM;

#endif // __cplusplus
