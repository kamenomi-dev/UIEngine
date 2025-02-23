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

using namespace Engine;

using namespace Logic;

unique_ptr<UIManager> UIManager::_selfInstance{};

// @brief Initializes window with its children.
void UIManager::InitializeWindow(vector<Components::Window*> children) {
    for (auto& child : children) {
        child->Initialize();
    }

    _windowList = children;
}

LRESULT UIManager::WindowsMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static auto& UIManager     = UIManager::Get();
    auto&        currentWindow = Components::Window::GetWindowMap()[hWnd];
    auto         bNoop         = false;

    if (currentWindow != nullptr) {
        const auto compResult = currentWindow->_Native_ComponentMessageProcessor(uMsg, wParam, lParam, bNoop);

        if (bNoop) {
            return compResult;
        }
    }

    if (uMsg == WM_ERASEBKGND) {
        return 0;
    }

    LRESULT dwmResult{};
    if (DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &dwmResult)) {
        return dwmResult;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}