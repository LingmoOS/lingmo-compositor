// SPDX-FileCopyrightText: 2026 LingmoOS Contributors
// SPDX-License-Identifier: GPL-3.0-or-later

// The compositor's components live in Lingmo::Scene, Lingmo::Clipboard,
// Lingmo::Display, ... but the sources name them with lower-case prefixes
// (scene::, clipboard::, display::, ...). Declare the namespaces first, so the
// aliases below have a target even at the top of a translation unit, then give
// the lower-case names a target at global scope. Included in every source by
// the project's compile options.
#pragma once

#ifdef __cplusplus

namespace Lingmo {
namespace Accelerators {}
namespace Auth {}
namespace Backend {}
namespace Clipboard {}
namespace Compositor {}
namespace ComputerControl {}
namespace Core {}
namespace Cursor {}
namespace Decoration {}
namespace Display {}
namespace Scene {
namespace Motion {}
}
namespace Session {}
namespace Shell {}
namespace Switcher {}
} // namespace Lingmo

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
namespace ui = Lingmo::ui;
namespace wm = Lingmo::WM;

namespace Lingmo::Scene {
namespace motion = Motion;
} // namespace Lingmo::Scene

#endif // __cplusplus
