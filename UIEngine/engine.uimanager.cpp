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

// UIManager Initialization

UIManager* UIManager::__hInstance{nullptr};

UIManager* UIManager::Initialize(HINSTANCE hInstance) {
    __hInstance = new UIManager(hInstance);

    return __hInstance;
}

UIManager* UIManager::Get() {
    CHECK_RESULT_BOOL(__hInstance);

    return __hInstance;
}

UIManager::UIManager(HINSTANCE hInstance) : _hProcessInstance(hInstance), _windowMap(nullptr), _mainWindowMap(nullptr) {
    this->_windowMap     = new unordered_map<HWND, Component::CWindow*>;
    this->_mainWindowMap = new unordered_map<HWND, Component::CWindow*>;

    Engine::Initialize(hInstance);
}

UIManager::~UIManager() {
    if (_windowMap != nullptr) {
        delete _windowMap;
        _windowMap = nullptr;
    }

    if (_mainWindowMap != nullptr) {
        delete _mainWindowMap;
        _mainWindowMap = nullptr;
    }
}

// UIManager Property Getter/Setter

HINSTANCE UIManager::GetProcessInstance() const { return _hProcessInstance; }

// UIManager Window Creation

CWindow* UIManager::CreateCentralWindow(wstring titleText, wstring className, Size windowSize, CWindow* parentWindow) {
    DEVMODEW devMode{};
    devMode.dmSize = sizeof DEVMODEW;

    EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &devMode);

    const auto screenWidth  = devMode.dmPelsWidth | 1920;  // default?
    const auto screenHeight = devMode.dmPelsHeight | 1080; // default?

    Rect windowRect = {
        (int)std::floor((screenWidth - windowSize.Width) / 2),
        (int)std::floor((screenHeight - windowSize.Height) / 2),
        windowSize.Width,
        windowSize.Height
    };


    return CreateNormalWindow(titleText, className, windowRect, parentWindow);
}

CWindow* UIManager::CreateNormalWindow(wstring titleText, wstring className, Rect windowRect, CWindow* parentWindow) {
    auto newWindow = new CWindow({
        {L"titleText",    (titleText)     },
        {L"classText",    (className)     },
        {L"windowRect",   Rect(windowRect)},
        {L"parentWindow", parentWindow    }
    });

    // Default is main for first main.

    if (((_windowMap->at(newWindow->GetWindowHandle()) == nullptr) && _windowMap->size() == 1)
        || _windowMap->size() == 0) {
        newWindow->SetPropertyIfNotExistByValue(L"isOwnerWindow", new bool(true));
    }

    __InsertWindowMap(newWindow->GetWindowHandle(), newWindow);
    return newWindow;
}

void UIManager::__InsertWindowMap(HWND hWnd, Component::CWindow* pWindow) {
    CHECK_RESULT_BOOL(pWindow);

    const auto selfWindowHandle = pWindow->GetWindowHandle();
    auto       z                = pWindow->GetPropertyTyped<bool>(L"isOwnerWindow");
    if (pWindow->GetPropertyTyped<bool>(L"isOwnerWindow")) {
        (*_mainWindowMap)[selfWindowHandle] = pWindow;
    }

    (*_windowMap)[selfWindowHandle]     = pWindow;
}

// UIManager Message Processor

LRESULT UIManager::WindowsMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    static auto& pUIManager = *UIManager::Get();
    auto&        pWindow    = (*pUIManager._windowMap)[hWnd];
    auto         bNoop      = false;

    static Point   ptMouse{};
    static wstring hoveredCompLabel{};

    if (uMsg == WM_MOUSEMOVE) {
        ptMouse = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        auto result = pWindow->GetComponentTree()->TryHitTest(ptMouse);

        const auto finalWindow = result.at(0);
        if (finalWindow != nullptr) {
            hoveredCompLabel = finalWindow->GetComponentLabel();
        }

        RECT rect{};
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = 200;
        rect.bottom = 32;
        InvalidateRect(hWnd, &rect, 1);

        rect.left   = ptMouse.X - 4;
        rect.top    = ptMouse.Y - 8;
        rect.right  = ptMouse.X + 4;
        rect.bottom = ptMouse.Y + 8;
        InvalidateRect(hWnd, &rect, 1);
    }

    if (uMsg == WM_CLOSE) {
        auto& pMainWindowMap = *(pUIManager._mainWindowMap);

        if (pMainWindowMap.find(hWnd) != pMainWindowMap.end()) {
            pMainWindowMap.erase(hWnd);
        }

        if (pMainWindowMap.size() == 0) {
            for (auto& window : *(pUIManager._windowMap)) {
                if (window.second != nullptr) {
                    delete window.second;
                }
            }

            PostQuitMessage(NULL);
        }

        return NULL;
    }

    if (uMsg == WM_SIZE) {
        pWindow->__Native_SetWindowSize({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});

        return NULL;
    }

    if (uMsg == WM_MOVE) {
        pWindow->__Native_SetWindowPosition({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});

        return NULL;
    }

    // Todo, how?
    if (uMsg == WM_PAINT) {
        pWindow->__Native_ComponentMessageProcessor(hWnd, uMsg, wParam, lParam, bNoop);
        return NULL;
    }

    if (uMsg == WM_ERASEBKGND) {
        return TRUE;
    }

    LRESULT dwmResult{};
    if (DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &dwmResult)) {
        return dwmResult;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

WPARAM UIManager::StartMessageLoop() {
    MSG msgStruct{};

    while (GetMessage(&msgStruct, nullptr, 0, 0)) {
        TranslateMessage(&msgStruct);
        DispatchMessage(&msgStruct);
    }

    Uninitialize();

    return msgStruct.wParam;
}
