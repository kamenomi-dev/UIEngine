/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

namespace Engine::Components {

// ==============================
// All of component flags
// ==============================

enum class UIENGINE_API ComponentStatusFlags : UINT { None = FLAG_INDEX(0), Visible = FLAG_INDEX(1), Disable = FLAG_INDEX(2) };

enum class UIENGINE_API WindowFrameFlags : UINT { None = FLAG_INDEX(0), Central = FLAG_INDEX(1), Borderless = FLAG_INDEX(2) };

// ==============================
// All of component messages
// ==============================

enum class UIENGINE_API ComponentMessages : UINT { First, Destroy, MouseHover, MouseLeave, Paint, Last };

// enum class UIENGINE_API WindowMessages : UINT { WindowBegin = (UINT)ComponentMessages::First + 1, WindowLast };

} // namespace Engine::Components


#endif // !__ENGINE_ENUM_H__
