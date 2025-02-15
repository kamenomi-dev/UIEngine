///* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine::Component {

enum class WindowFrameFlag { None = FLAG_INDEX(0), Central = FLAG_INDEX(1), Borderless = FLAG_INDEX(2) };

struct WindowPropertyDataType {
    UINT FrameFlag; // More see WindowFrameFlag.

    HWND       WindowHandle;
    Rect       WindowRect; // It is not as same as ComponentRect in BasePropertyDataType.
    wstring    WindowTitleText;
    wstring    WindowClassText;
    WNDCLASSEX WindowClassInformation;
};

class UIENGINE_API CWindow : public CBase {
public:
    CWindow(WindowFrameFlag);
    ~CWindow();

    void Initialize();

private:
    void _UpdateSwapBufferSize() {
        if (not _swapBuffer) {
            return;
        }

        unordered_map<CWindow*, Size> lastSizeMap{};
        auto&                         lastSize = lastSizeMap[this];
    }

private:
    bool                           _initialized{false};
    unique_ptr<Render::SwapBuffer> _swapBuffer;

    /*
     *
     *   Component Property Part.
     *
     */

private:
    WindowPropertyDataType _windowProperty{};

public:
    WindowPropertyDataType&       _Property_GetWindowPropertyData() { return _windowProperty; }
    const WindowPropertyDataType& _Property_GetWindowPropertyData() const { return _windowProperty; }

    const HWND _Property_GetWindowHandle() const { return WindowProperty.WindowHandle; };

    const wstring& _Property_GetWindowClassText() const { return WindowProperty.WindowClassText; };
    const wstring& _Property_SetWindowClassText(const wstring& newClass) { return WindowProperty.WindowClassText = newClass; };

    const wstring& _Property_GetWindowTitleText() const { return WindowProperty.WindowClassText; };
    const wstring& _Property_SetWindowTitleText(const wstring& newTitle) { return WindowProperty.WindowClassText = newTitle; };


    Rect&       _Property_GetWindowRect() { return WindowProperty.WindowRect; }
    const Rect& _Property_GetWindowRect() const { return WindowProperty.WindowRect; }
    Rect&       _Property_SetWindowRect(Rect newRect) {
        // Todo! Support dynamic render.
        WindowProperty.WindowRect = newRect;

        return WindowProperty.WindowRect;
    }

    Size _Property_GetWindowSize() const {
        Size returnSize{};

        ComponentRect.GetSize(&returnSize);

        return returnSize;
    }
    Size _Property_SetWindowSize(Size newSize) {
        // Todo! Support dynamic render.
        WindowRect.Width  = newSize.Width;
        WindowRect.Height = newSize.Height;

        return newSize;
    }

    Point _Property_GetWindowPosition() const {
        Point returnPosition{};

        WindowRect.GetLocation(&returnPosition);

        return returnPosition;
    }
    Point _Property_SetWindowPosition(Point newPosition) {
        // Todo! Support dynamic render.
        WindowRect.X = newPosition.X;
        WindowRect.Y = newPosition.Y;

        return newPosition;
    }

    COMPONENT_PROPERTY(_Property_GetWindowPropertyData, _Property_SetWindowPropertyData)
    WindowPropertyDataType WindowProperty;

    COMPONENT_PROPERTY_GETTER_ONLY(_Property_GetWindowHandle) HWND WindowHandle;
    COMPONENT_PROPERTY(_Property_GetWindowClassText, _Property_SetWindowClassText)
    wstring WindowClassText;
    COMPONENT_PROPERTY(_Property_GetWindowTitleText, _Property_SetWindowTitleText)
    wstring WindowTitleText;
    COMPONENT_PROPERTY(_Property_GetWindowRect, _Property_SetWindowRect) Rect WindowRect;
    COMPONENT_PROPERTY(_Property_GetWindowSize, _Property_SetWindowSize) Size WindowSize;
    COMPONENT_PROPERTY(_Property_GetWindowPosition, _Property_SetWindowPosition) Point WindowPosition;

    COMPONENT_PROPERTY_GETTER_ONLY(_Property_GetComponentPosition) Point ComponentPosition;
};

} // namespace Engine::Component

#endif // !__COMPONENT_WINDOW_H__

// #pragma once
//
// #ifndef __COMPONENT_WINDOW_H__
// #define __COMPONENT_WINDOW_H__
//
// namespace Engine::Component {
//
// class UIENGINE_API CWindow : public CBase {
// public:
//     CWindow(vector<Utils::PropertyPair>);
//     ~CWindow() {
//         DestroyWindow(_windowHandle);
//         RegisterClassExW(&_windowClassInfo);
//
//         _windowClassInfo = {};
//     }
//
// public:
//     unique_ptr<Logic::CComponentTree>& GetComponentTree() { return _componentTree; };
//     // User musn't change the position via using this method. Or£¿
//     Point _Property_SetComponentPosition() const = delete;
//
//     virtual wstring _Property_GetComponentClass() const { return L"Component.Window"s; }
//
//     bool IsOwnerWindow() const { return GetPropertyTyped<bool>(L"isOwnerWindow"); }
//     HWND GetWindowHandle() const {
//         if (!_windowHandle) {
//             throw runtime_error("Window hasn't created yet! ");
//         }
//
//         return _windowHandle;
//     }
//     void SetWindowOwner(CWindow* pOwner) {
//         SetPropertyByValue(L"parentWindow", pOwner);
//         SetPropertyByValue(L"isOwnerWindow", pOwner != nullptr);
//     }
//
//
//     Render::SwapBuffer& GetWindowSwapBuffer() { return *_renderSwapBuffer; }
//
//     void Render(Gdiplus::Graphics&);
//
//     LRESULT
//     _Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);
//
//     void _Native_SetWindowSize(Size newSize) {
//
//         ComponentSize      = newSize;
//         _windowRect.Width  = newSize.Width;
//         _windowRect.Height = newSize.Height;
//
//         _UpdateWindowSwapBuffer();
//     }
//
//     void _Native_SetWindowPosition(Point newPosition) {
//         _windowRect.X = newPosition.X;
//         _windowRect.Y = newPosition.Y;
//
//         _UpdateWindowSwapBuffer();
//     }
//
// private:
//     HWND                              _windowHandle;
//     WNDCLASSEXW                       _windowClassInfo;
//     unique_ptr<Logic::CComponentTree> _componentTree;
//     unique_ptr<Render::SwapBuffer>    _renderSwapBuffer;
//
//     void _RegisterClass();
//     HWND _CreateWindow();
//     void _UpdateWindowSwapBuffer() {
//         static unordered_map<CWindow*, Size> lastSizeMap{};
//         auto&                                lastSize = lastSizeMap[this];
//
//         if (not lastSize.Equals(ComponentSize)) {
//             _renderSwapBuffer->RefreshSize();
//
//             lastSize = ComponentSize;
//         }
//     }
//
// private:
//     // Property
//
//     Rect _windowRect{};
//
// public:
//     COMPONENT_PROPERTY(_Property_GetWindowRect, _Property_SetWindowRect) Rect WindowRect;
//     COMPONENT_PROPERTY(_Property_GetWindowSize, _Property_SetWindowSize) Size WindowSize;
//     COMPONENT_PROPERTY(_Property_GetWindowPosition, _Property_SetWindowPosition) Point WindowPosition;
//
//     Rect& _Property_GetWindowRect() { return _windowRect; }
//     void  _Property_SetWindowRect(Rect newRect) {
//         WindowSize     = Size{newRect.Width, newRect.Height};
//         WindowPosition = Point{newRect.X, newRect.Y};
//     }
//
//     Size _Property_GetWindowSize() const { return {_windowRect.Width, _windowRect.Height}; }
//     void _Property_SetWindowSize(Size newSize) {
//         ComponentSize      = newSize;
//         _windowRect.Width  = newSize.Width;
//         _windowRect.Height = newSize.Height;
//
//         _UpdateWindowSwapBuffer();
//
//         SetWindowPos(
//             _windowHandle,
//             nullptr,
//             _windowRect.X,
//             _windowRect.Y,
//             _windowRect.Width,
//             _windowRect.Height,
//             SWP_NOOWNERZORDER
//         );
//     }
//
//     Point _Property_GetWindowPosition() const { return {_windowRect.X, _windowRect.Y}; }
//     void  _Property_SetWindowPosition(Point newPosition) {
//         _windowRect.X = newPosition.X;
//         _windowRect.Y = newPosition.Y;
//
//         _UpdateWindowSwapBuffer();
//
//         SetWindowPos(
//             _windowHandle,
//             nullptr,
//             _windowRect.X,
//             _windowRect.Y,
//             _windowRect.Width,
//             _windowRect.Height,
//             SWP_NOOWNERZORDER
//         );
//     }
// };
// } // namespace Engine::Component
//
// #endif // !__COMPONENT_WINDOW_H__
