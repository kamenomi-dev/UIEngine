/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UTILS_FLAGS_H__
#define __ENGINE_UTILS_FLAGS_H__

namespace Engine::Utils::Flags {

template <typename Ty>
inline [[nodiscard]] bool HasFlag(_In_ unsigned int flags, _In_ Ty curr) {
    return (flags & (unsigned int)curr) != 0;
}

template <typename Ty>
inline [[nodiscard]] void CombineFlag(_Inout_ unsigned int& outFlag, _In_ std::initializer_list<Ty> flags) {
    for (auto flag : flags) {
        outFlag |= static_cast<unsigned int>(flag);
    }
}

} // namespace Engine::Utils::Flags

#endif // !__ENGINE_UTILS_FLAGS_H__