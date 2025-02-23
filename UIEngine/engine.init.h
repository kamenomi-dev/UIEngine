/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_INIT_H__
#define __ENGINE_INIT_H__

namespace Engine {

UIENGINE_API extern void    InitializeWindow(vector<Components::Window*>);
UIENGINE_API extern LRESULT WindowsMessageProcessor(HWND, UINT, WPARAM, LPARAM);

} // namespace Engine

#endif // !__ENGINE_INIT_H__
