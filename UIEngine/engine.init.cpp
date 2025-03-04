/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *    Gdiplus UI, using gdiplus, is a UI library of Windows platform which
 *    is based on C++.
 *     Copyright (C) 2025  Project Contributors
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *     any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "pch.h"
#include "engine.h"

using namespace Engine::Components;

// @brief Initializes window with its children.
UIENGINE_API void Engine::InitializeWindow(vector<Window*> children) {
    for (auto& child : children) {
        child->Initialize();
    }
}

UIENGINE_API LRESULT Engine::WindowsMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto windowInfo = Window::GetWindowMap().find(hWnd);
    auto bNoop      = false;

    if (uMsg == WM_ERASEBKGND) {
        return 0;
    }

    if (windowInfo != Window::GetWindowMap().end()) {
        const auto currentWindow = windowInfo->second.ThisWindow;
        const auto compResult    = currentWindow->_Native_ComponentMessageProcessor(uMsg, wParam, lParam, bNoop);

        if (bNoop) {
            return compResult;
        }
    }

    LRESULT dwmResult{};
    if (DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &dwmResult)) {
        return dwmResult;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}