/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

#define UIE_ENUM_BIT_FLAGS(Type)                                                                                                                                                   \
    FORCEINLINE constexpr Type  operator&(Type a, Type b) { return Type((std::underlying_type_t<Type>)a & (std::underlying_type_t<Type>)b); }                                        \
    FORCEINLINE constexpr Type  operator|(Type a, Type b) { return Type((std::underlying_type_t<Type>)a | (std::underlying_type_t<Type>)b); }                                        \
    FORCEINLINE constexpr Type  operator~(Type a) { return Type(~(std::underlying_type_t<Type>)a); }                                                                                 \
    FORCEINLINE constexpr Type  operator^(Type a, Type b) { return Type((std::underlying_type_t<Type>)a ^ (std::underlying_type_t<Type>)b); }                                        \
    FORCEINLINE constexpr Type& operator&=(Type& a, Type b) {                                                                                                                        \
        a = a & b;                                                                                                                                                                 \
        return a;                                                                                                                                                                  \
    }                                                                                                                                                                              \
    FORCEINLINE constexpr Type& operator|=(Type& a, Type b) {                                                                                                                        \
        a = a | b;                                                                                                                                                                 \
        return a;                                                                                                                                                                  \
    }                                                                                                                                                                              \
    FORCEINLINE constexpr Type& operator^=(Type& a, Type b) {                                                                                                                        \
        a = a ^ b;                                                                                                                                                                 \
        return a;                                                                                                                                                                  \
    }

#define UIE_ENUM_BIT_FLAGS_FRIEND(Type)                                                                                                                                            \
    friend constexpr Type  operator&(Type a, Type b);                                                                                                                              \
    friend constexpr Type  operator|(Type a, Type b);                                                                                                                              \
    friend constexpr Type  operator~(Type a);                                                                                                                                      \
    friend constexpr Type  operator^(Type a, Type b);                                                                                                                              \
    friend constexpr Type& operator&=(Type& a, Type b);                                                                                                                            \
    friend constexpr Type& operator|=(Type& a, Type b);                                                                                                                            \
    friend constexpr Type& operator^=(Type& a, Type b);


namespace Engine::Components {

// ==============================
// All of component flags
// ==============================

enum class CompStatus : UINT {
    None    = 0u,
    Visible = FLAG_INDEX(0),
    Disable = FLAG_INDEX(1)
};
UIE_ENUM_BIT_FLAGS(CompStatus);

enum class WndFrame : UINT {
    None       = 0u,
    Central    = FLAG_INDEX(0),
    Borderless = FLAG_INDEX(1)
};
UIE_ENUM_BIT_FLAGS(WndFrame);

// ==============================
// All of component messages
// ==============================

enum class UIENGINE_API ComponentMessages : UINT {
    First,
    Destroy,
    MouseHover,
    MouseLeave,
    Paint,
    Last
};

// enum class UIENGINE_API WindowMessages : UINT { WindowBegin = (UINT)ComponentMessages::First + 1, WindowLast };

} // namespace Engine::Components


#endif // !__ENGINE_ENUM_H__
