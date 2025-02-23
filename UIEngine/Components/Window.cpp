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

unordered_map<HWND, Window*> Window::WindowMap{};

// ---

void Window::Initialize() {
    for (auto& window : WindowMap) {
        if (window.second == this) {
            throw std::logic_error("Window has initialized already. ");
        }
    }

    {
        WNDCLASSEX classInfo{NULL};
        classInfo.cbSize        = sizeof WNDCLASSEX;
        classInfo.hInstance     = Engine::ProcessInstance;
        classInfo.lpfnWndProc   = &UIManager::WindowsMessageProcessor;
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


    _swapBuffer             = make_unique<SwapBuffer>(WindowHandle);
    _initialized            = true;

    componentTree           = make_unique<CComponentTree>(this);
    WindowMap[WindowHandle] = this;

    // if (WindowMap.size() == 1) {
    //     this->WindowProperty.IsOwnerWindow = true;
    // }
}

void Window::_Native_UpdateWindowSize(Size newSize) {
    _componentData.ComponentSize = _windowData.WindowSize = newSize;
    _UpdateSwapBufferSize();
}

void    Window::_Native_UpdateWindowPosition(Point newPosition) { _windowData.WindowPosition = newPosition; }

LRESULT Window::_Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& isReturn) {

    if (uMsg == WM_CLOSE) {
        // if (WindowMap.size() == NULL) return NULL;

        //// 因为第一个，即首个创建的窗口为 Owner 窗口。
        // const auto isOwnerWindowExist = WindowMap.begin()->second->WindowProperty.IsOwnerWindow;

        // if (!WindowProperty.IsOwnerWindow) {
        //     if (isOwnerWindowExist) {
        //         return NULL;
        //     }
        //     Component::_Native_ComponentMessageProcessor(WM_CLOSE, NULL, NULL, isReturn);
        //     return NULL;
        // }

        Component::_Native_ComponentMessageProcessor(WM_CLOSE, NULL, NULL, isReturn);
        WindowMap.erase(WindowHandle);

        if (WindowMap.empty()) {
            PostQuitMessage(NULL);
        }

        isReturn = true;
        delete this;
    }

    if (uMsg == WM_PAINT) {
        PAINTSTRUCT paintStruct{};
        BeginPaint(WindowHandle, &paintStruct);

        Rect targetRect{RectToGpRect(paintStruct.rcPaint)};
        auto coveredComponents = componentTree->TryHitTest(targetRect) | std::views::reverse | std::views::filter([](const auto& p) { return p != nullptr; });

        // -
        Gdiplus::Graphics graphics{_swapBuffer->GetDrawContext()};

        for (const auto& component : coveredComponents) {
            const auto status = graphics.Save();
            {
                graphics.SetClip(Rect(component->ComponentPosition, component->ComponentSize));
                component->_Native_TransformMessageProcessor(ComponentMessages::Paint, NULL, (LPARAM)&graphics);
            }
            graphics.Restore(status);
        }

        if (HasFlag(FrameFlags, WndFrame::Borderless)) {
            _swapBuffer->Present();
        } else {
            _swapBuffer->Present(paintStruct.hdc);
        }
        EndPaint(WindowHandle, &paintStruct);
        isReturn = true;
    }

    if (uMsg == WM_SIZE) {
        _Native_UpdateWindowSize({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
        isReturn = true;
    }

    if (uMsg == WM_MOVE) {
        _Native_UpdateWindowPosition({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
        isReturn = true;
    }

    if (uMsg == WM_MOUSEMOVE) {
        static unordered_map<HWND, Component*> lastComponentMap{};

        // --
        static Component* lastComponent = nullptr;

        const Point       mousePoint    = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        const auto        nextComponent = componentTree->TryHitTest(mousePoint)[0];

        if (lastComponent == nextComponent) {
            return NULL;
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

    return NULL;
}

//
// Window::Window(vector<Utils::PropertyPair> pairs) : Component(pairs) {
//    SetPropertyIfNotExistByValue(L"titleText", L"Window."s);
//    SetPropertyIfNotExistByValue(L"classText", L"UIEngine.Window"s);
//    SetPropertyIfNotExistByValue(L"windowRect", make_any<Rect>(0, 0, 800, 600));
//    SetPropertyIfNotExistByValue(L"parentWindow", nullptr);
//    SetPropertyIfNotExistByValue(L"isOwnerWindow", false);
//
//    _RegisterClass();
//
//    const auto& rect = GetPropertyTyped<Rect>(L"windowRect");
//    ComponentSize    = Size({rect.Width, rect.Height});
//
//    const auto& pParent = GetPropertyTyped<Window*>(L"parentWindow");
//    if (pParent == nullptr) {
//        SetPropertyByValue(L"isOwnerWindow", true);
//    }
//
//    _componentTree    = make_unique<CComponentTree>(this);
//    _renderSwapBuffer = make_unique<SwapBuffer>(_CreateWindow());
//}
//
// void Window::_RegisterClass() {
//    WNDCLASSEXW classInfo{};
//    classInfo.cbSize        = sizeof WNDCLASSEXW;
//    classInfo.hInstance     = Engine::ProcessInstance;
//    classInfo.lpfnWndProc   = &UIManager::WindowsMessageProcessor;
//    classInfo.lpszClassName = GetPropertyTyped<wstring>(L"classText").c_str();
//
//    (RegisterClassExW(&classInfo));
//    const auto err   = GetLastError();
//    _windowClassInfo = classInfo;
//}
//
// HWND Window::_CreateWindow() {
//    const auto& rect    = GetPropertyTyped<Rect>(L"windowRect");
//    const auto& pParent = GetPropertyTyped<Window*>(L"parentWindow");
//
//    if (pParent == nullptr) {
//        SetPropertyByValue(L"isOwnerWindow", true);
//    }
//
//    HWND hParent{pParent ? pParent->GetWindowHandle() : nullptr};
//
//    const auto hWnd = CreateWindowExW(
//        WS_EX_APPWINDOW,
//        GetPropertyTyped<wstring>(L"classText").c_str(),
//        GetPropertyTyped<wstring>(L"titleText").c_str(),
//        WS_OVERLAPPEDWINDOW,
//        rect.X,
//        rect.Y,
//        rect.Width,
//        rect.Height,
//        hParent,
//        nullptr,
//        UIManager::Get().GetProcessInstance(),
//        nullptr
//    );
//
//
//    const auto err = GetLastError();
//    CHECK_RESULT_BOOL(_windowHandle = hWnd);
//
//    return hWnd;
//}
//
// void Window::Render(Gdiplus::Graphics& grap) { Component::Render(grap); }
//
// LRESULT
// Window::_Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& isReturn) {
//
//    if (uMsg == WM_PAINT) {
//        PAINTSTRUCT paintStruct{};
//        const HDC   hTargetDC = ::BeginPaint(hWnd, &paintStruct);
//        const auto& paintRect = paintStruct.rcPaint;
//
//        auto&             swapBuffer = GetWindowSwapBuffer();
//        Gdiplus::Graphics graphics{swapBuffer.GetDrawContext()};
//
//        const Gdiplus::Rect renderableRect{
//            paintRect.left,
//            paintRect.top,
//            paintRect.right - paintRect.left,
//            paintRect.bottom - paintRect.top
//        };
//
//        auto coveredComponents = _componentTree->TryHitTest(renderableRect) | std::views::reverse
//                               | std::views::filter([](const auto& p) { return p != nullptr; });
//
//        for (const auto& pComponent : coveredComponents) {
//            if (not pComponent->ComponentVisible) {
//                continue;
//            }
//
//            const auto lastState = graphics.Save();
//            {
//                graphics.SetClip(pComponent->ComponentRect);
//                pComponent->_Native_TransformMessageProcessor(CM_PAINT, 0, (LPARAM)&graphics);
//            }
//            graphics.Restore(lastState);
//        }
//
//        swapBuffer.Present(hTargetDC);
//        ::EndPaint(hWnd, &paintStruct);
//
//        return NULL;
//    }
//
//    return Component::_Native_ComponentMessageProcessor(uMsg, wParam, lParam, isReturn);
//}
