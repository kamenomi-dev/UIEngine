#pragma once

#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

namespace Engine {
namespace Enum {
namespace Component {
typedef enum __enumComponentBaseMessage { CM_BASE_FIRST, CM_BASE_PAINT, CM_BASE_LAST } ComponentBaseMessageEnum;

typedef enum __enumComponentWindowMessage {
    CM_WINDOW_BEGIN = CM_BASE_LAST + 1,
    CM_WINDOW_LAST
} ComponentWindowMessageEnum;
} // namespace Component
}; // namespace Enum

using namespace Enum::Component;
} // namespace Engine

#endif // !__ENGINE_ENUM_H__
