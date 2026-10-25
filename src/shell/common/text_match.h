// Copyright 2026 LingmoOS Contributors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#pragma once


#include <string>
#include <string_view>

namespace Lingmo::Shell {

// Case-insensitive search: each character compared by its simple Unicode case
// folding, so "É" finds "é" and "Σ" finds "ς".
bool containsIgnoringCase(QStringView text, QStringView needle);

// Unicode lower case, not only ASCII.
std::string toLower(QStringView text);

} // namespace Lingmo::Shell

