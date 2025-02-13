/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

namespace Engine {
namespace Enum::Component {
enum ComponentBaseMessageEnum { CM_FIRST, CM_MOUSE_HOVER, CM_MOUSE_LEAVE, CM_PAINT, CM_LAST };

enum ComponentWindowMessageEnum { CM_WINDOW_BEGIN = CM_LAST + 1, CM_WINDOW_LAST };
} // namespace Enum::Component

using namespace Enum::Component;
} // namespace Engine

#endif // !__ENGINE_ENUM_H__
