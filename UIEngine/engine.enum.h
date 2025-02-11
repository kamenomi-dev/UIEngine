/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

namespace Engine {
namespace Enum {
namespace Component {
typedef enum __enumComponentBaseMessage {
    CM_FIRST,
    CM_MOUSE_HOVER,
    CM_MOUSE_LEAVE,
    CM_PAINT,
    CM_LAST
} ComponentBaseMessageEnum;

typedef enum __enumComponentWindowMessage { CM_WINDOW_BEGIN = CM_LAST + 1, CM_WINDOW_LAST } ComponentWindowMessageEnum;
} // namespace Component
}; // namespace Enum

using namespace Enum::Component;
} // namespace Engine

#endif // !__ENGINE_ENUM_H__
