/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UTIL_H__

namespace Engine::Utils {

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

inline [[nodiscard]] Rect RectToGpRect(const RECT& rc) { return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top); }

} // namespace Engine::Utils

#endif // !__ENGINE_UTIL_H__