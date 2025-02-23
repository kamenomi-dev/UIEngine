/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UTILS_FLAGS_H__
#define __ENGINE_UTILS_FLAGS_H__

namespace Engine::Utils {

template <class T>
concept ccpIsIntOrEnum = std::is_integral_v<T> || std::is_enum_v<T>;

template <ccpIsIntOrEnum T, bool = std::is_enum_v<T>>
struct UnderlyingType {
    using Type = std::underlying_type_t<T>;
};

template <ccpIsIntOrEnum T>
struct UnderlyingType<T, false> {
    using Type = T;
};

template <ccpIsIntOrEnum T>
using UnderlyingType_T = UnderlyingType<T>::Type;

namespace Flags {

template <ccpIsIntOrEnum T, ccpIsIntOrEnum U>
inline [[nodiscard]] bool HasFlag(T flags, U curr) {
    return (UnderlyingType_T<T>(flags) & UnderlyingType_T<U>(curr)) != 0;// XXX: 严格检测需要 == flags（也可不用）
}

// DEPRECATED: use &
template <typename Ty>
inline [[nodiscard]] void CombineFlag(_Inout_ unsigned int& outFlag, _In_ std::initializer_list<Ty> flags) {
    for (auto flag : flags) {
        outFlag |= static_cast<unsigned int>(flag);
    }
}

} // namespace Flags
} // namespace Engine::Utils

#endif // !__ENGINE_UTILS_FLAGS_H__