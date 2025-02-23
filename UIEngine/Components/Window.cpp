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
#include "../engine.h"

using namespace Engine::Utils::Flags;
using namespace Engine::Utils::Graph;
using namespace Engine::Logic;
using namespace Engine::Components;

unordered_map<HWND, Window*> Window::windowMap{};

// ---

void Window::Initialize() {
    for (auto& window : windowMap) {
        if (window.second == this) {
            throw std::logic_error("Window has initialized already. ");
        }
    }

    {
        WNDCLASSEX classInfo{NULL};
        classInfo.cbSize        = sizeof WNDCLASSEX;
        classInfo.hInstance     = Engine::ProcessInstance;
        classInfo.lpfnWndProc   = &WindowsMessageProcessor;
        classInfo.lpszClassName = _windowData.WindowClassText.c_str();

        RegisterClassEx(&classInfo);
        _windowData.WindowClassInformation = classInfo;
    }

    {
        Rect  actualRect  = Rect{WindowPosition, WindowSize};
        DWORD actualStyle = NULL;

        if (Visible) {
            actualStyle |= WS_VISIBLE;
        }

        if (HasFlag(_windowData.FrameFlag, WndFrame::Central)) {
            const auto screenWidth  = GetSystemMetrics(SM_CXSCREEN);
            const auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

            actualRect = {((int)screenWidth - WindowSize.Width) / 2, ((int)screenHeight - WindowSize.Height) / 2, WindowSize.Width, WindowSize.Height};
        }

        if (HasFlag(_windowData.FrameFlag, WndFrame::Borderless)) {
            actualStyle |= WS_POPUP;
        } else {
            actualStyle |= WS_OVERLAPPEDWINDOW;
        }

        _windowData.WindowHandle = CreateWindowExW(
            WS_EX_OVERLAPPEDWINDOW,
            WindowClassText.c_str(),
            WindowTitle.c_str(),
            actualStyle,
            actualRect.X,
            actualRect.Y,
            actualRect.Width,
            actualRect.Height,
            nullptr,
            nullptr,
            Engine::ProcessInstance,
            nullptr
        );

        if (WindowHandle == nullptr) {
            throw std::runtime_error("Create window failed! ");
        }
    }


    _initialized            = true;
    windowMap[WindowHandle] = this;

    swapBuffer    = make_unique<SwapBuffer>(WindowHandle);
    componentTree = make_unique<ComponentTree>(this)
}

void Window::_Native_UpdateWindowSize(Size newSize) {
    _componentData.ComponentSize = _windowData.WindowSize = newSize;
    _UpdateSwapBufferSize();
}

void Window::_Native_UpdateWindowPosition(Point newPosition) { _windowData.WindowPosition = newPosition; }

// ---

inline void OnPaint(Window* currWnd, UINT, WPARAM, LPARAM);
inline void OnMouseMove(Window* currWnd, UINT uMsg, WPARAM, LPARAM lParam);
LRESULT     Window::_Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& isReturn) {

    switch (uMsg) {
    case WM_SIZE: {
        _Native_UpdateWindowSize({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
        isReturn = true;
        break;
    }
    case WM_MOVE: {
        _Native_UpdateWindowPosition({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
        isReturn = true;
        break;
    }
    case WM_PAINT: {
        isReturn = true;

        break;
    }
    case WM_CLOSE: {
        Component::_Native_ComponentMessageProcessor(WM_CLOSE, NULL, NULL, isReturn);
        windowMap.erase(WindowHandle);

        if (windowMap.empty()) {
            PostQuitMessage(NULL);
        }

        isReturn = true;
        delete this;

        [[fallthrough]];
    }
    case WM_MOUSEMOVE: {
        OnMouseMove(this, uMsg, NULL, lParam);
        break;
    }
    }

    return NULL;
}

inline void OnPaint(Window* currWnd, UINT, WPARAM, LPARAM) {
    const auto  hWnd       = currWnd->WindowHandle;
    const auto& swapBuffer = currWnd->swapBuffer;

    PAINTSTRUCT paintStruct{};
    BeginPaint(hWnd, &paintStruct);

    const Rect invalidArea{RectToGpRect(paintStruct.rcPaint)};
    auto       coveredComponents = currWnd->componentTree->TryHitTest(invalidArea) | std::views::reverse | std::views::filter([](const auto& p) { return p != nullptr; });

    // -
    Gdiplus::Graphics graphics{swapBuffer->GetDrawContext()};

    for (const auto& component : coveredComponents) {
        const auto& status = graphics.Save();
        {
            graphics.SetClip(Rect(component->ComponentPosition, component->ComponentSize));
            component->_Native_TransformMessageProcessor(ComponentMessages::Paint, NULL, (LPARAM)&graphics);
        }
        graphics.Restore(status);
    }

    if (HasFlag(currWnd->FrameFlags, WndFrame::Borderless)) {
        swapBuffer->Present();
    } else swapBuffer->Present(paintStruct.hdc);

    EndPaint(hWnd, &paintStruct);
}

inline void OnMouseMove(Window* currWnd, UINT uMsg, WPARAM, LPARAM lParam) {
    static unordered_map<Window*, Component*> lastComponentMap{};

    if (uMsg == WM_CLOSE) {
        lastComponentMap.erase(currWnd);
        return;
    }

    // --
    static Component* lastComponent = lastComponentMap[currWnd];

    const Point       mousePoint    = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    const auto        nextComponent = currWnd->componentTree->TryHitTest(mousePoint)[0];

    if (lastComponent == nextComponent) {
        return;
    }

    if (lastComponent) {
        const auto& posComponent = lastComponent->ComponentPosition;
        const auto  ptMouse      = mousePoint - posComponent;

        lastComponent->_Native_TransformMessageProcessor(ComponentMessages::MouseLeave, 0, (LPARAM)&ptMouse);
    }

    if (nextComponent) {
        lastComponent = nextComponent;

        const auto& posComponent = nextComponent->ComponentPosition;
        const auto  ptMouse      = mousePoint - posComponent;

        nextComponent->_Native_TransformMessageProcessor(ComponentMessages::MouseHover, 0, (LPARAM)&ptMouse);
    }
}