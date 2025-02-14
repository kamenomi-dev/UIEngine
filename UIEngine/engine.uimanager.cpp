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
using namespace Component;

unique_ptr<UIManager> UIManager::_selfInstance{};

// UIManager Window Creation

CWindow* UIManager::CreateCentralWindow(wstring titleText, wstring className, Size windowSize, CWindow* parentWindow) {
    /*DEVMODEW devMode{.dmSize = sizeof DEVMODEW};
    EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &devMode);*/
    // HACK: 1.DPI感知；2.换用完善的居中判定
    const auto screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    const auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

    Rect windowRect = {
        ((int)screenWidth - windowSize.Width) / 2,
        ((int)screenHeight - windowSize.Height) / 2,
        windowSize.Width,
        windowSize.Height
    };

    return CreateNormalWindow(titleText, className, windowRect, parentWindow);
}

CWindow* UIManager::CreateNormalWindow(wstring titleText, wstring className, Rect windowRect, CWindow* parentWindow) {
    auto newWindow = new CWindow({
        {L"titleText",    (titleText)     },
        {L"classText",    (className)     },
        {L"parentWindow", parentWindow    }
    });

    newWindow->WindowRect = windowRect;

    // Default is main for first main.

    if (((_windowMap.at(newWindow->GetWindowHandle()) == nullptr) && _windowMap.size() == 1)
        || _windowMap.size() == 0) {
        newWindow->SetPropertyIfNotExistByValue(L"isOwnerWindow", new bool(true));
    }

    _InsertWindowMap(newWindow->GetWindowHandle(), newWindow);
    return newWindow;
}

void UIManager::_InsertWindowMap(HWND hWnd, Component::CWindow* ptrWindow) {
    CHECK_RESULT_BOOL(ptrWindow);

    const auto selfWindowHandle = ptrWindow->GetWindowHandle();
    if (ptrWindow->GetPropertyTyped<bool>(L"isOwnerWindow")) {
        _mainWindowMap[selfWindowHandle] = ptrWindow;
    }

    _windowMap[selfWindowHandle] = ptrWindow;
}

// UIManager Message Processor

static void OnMouseMove(HWND hCurrentWindow, LPARAM lParam) {
    static unordered_map<HWND, CBase*> lastComponentMap{};

    auto& currentWindow = UIManager::Get().GetWindowMap()[hCurrentWindow];
    auto& lastComponent = lastComponentMap[hCurrentWindow];

    const Point ptCurrMouse    = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    const auto  pNextComponent = currentWindow->GetComponentTree()->TryHitTest(ptCurrMouse)[0];

    if (lastComponent == pNextComponent) {
        return;
    }

    if (lastComponent) {
        const auto posComponent = lastComponent->ComponentPosition;
        const auto ptMouse      = ptCurrMouse - posComponent;

        lastComponent->_Native_TransformMessageProcessor(CM_MOUSE_LEAVE, 0, (LPARAM)&ptMouse);
    }

    if (pNextComponent) {
        lastComponent = pNextComponent;

        const auto posComponent = pNextComponent->ComponentPosition;
        const auto ptMouse      = ptCurrMouse - posComponent;

        pNextComponent->_Native_TransformMessageProcessor(CM_MOUSE_HOVER, 0, (LPARAM)&ptMouse);
    }
}

LRESULT UIManager::WindowsMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    static auto& UIManager     = UIManager::Get();
    auto&        currentWindow = *UIManager._windowMap[hWnd];
    auto         bNoop         = false;

    if (uMsg == WM_MOUSEMOVE) {
        OnMouseMove(hWnd, lParam);
    }

    if (uMsg == WM_CLOSE) {

        auto& mainWindowMap = (UIManager._mainWindowMap);

        auto windowResult = mainWindowMap.find(hWnd);
        if (windowResult != mainWindowMap.end()) {

            auto& windowMap  = UIManager.GetWindowMap();
            auto& mainWindow = windowResult->second;

            mainWindow->_Native_ComponentMessageProcessor(hWnd, uMsg, NULL, NULL, bNoop);

            delete mainWindow;
            mainWindow = nullptr;
            windowMap.erase(hWnd);
            mainWindowMap.erase(hWnd);
        }

        if (mainWindowMap.size() == 0) {
            // UIManager.GetWindowMap().clear();

            for (auto& window : (UIManager._windowMap)) {
                if (window.second != nullptr) {
                    window.second->_Native_ComponentMessageProcessor(hWnd, uMsg, NULL, NULL, bNoop);
                    delete window.second;
                }
            }

            PostQuitMessage(0);
        }

        return 0;
    }

    if (uMsg == WM_SIZE) {
        currentWindow._Native_SetWindowSize({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});

        return 0;
    }

    if (uMsg == WM_MOVE) {
        currentWindow._Native_SetWindowPosition({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});

        return 0;
    }

    // Todo, how?
    if (uMsg == WM_PAINT) {
        currentWindow._Native_ComponentMessageProcessor(hWnd, uMsg, wParam, lParam, bNoop);

        return 0;
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