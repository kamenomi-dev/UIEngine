///* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine::Component {

enum class UIENGINE_API WindowFrameFlag : UINT { None = FLAG_INDEX(0), Central = FLAG_INDEX(1), Borderless = FLAG_INDEX(2) };

struct UIENGINE_API     WindowDataType {
    UINT       FrameFlag{(UINT)WindowFrameFlag::None};

    HWND       WindowHandle{nullptr};
    bool       IsOwnerWindow{false}; // Owner window is unique to all of windows.
    Size       WindowSize{800, 600};
    Point      WindowPosition{0, 0};
    wstring    WindowClassText{L"Component_Window"};
    wstring    WindowTitleText{L"视窗"};
    WNDCLASSEX WindowClassInformation{};
};

class UIENGINE_API Window : public ComponentBase {
  public:
    Window() : ComponentBase() {
        _componentData.ComponentSize     = {800, 600};
        _componentData.ComponentPosition = {0, 0};
    }
    ~Window() {
        if (!_initialized) {
            return;
        }

        DestroyWindow(WindowHandle);

        RegisterClassEx(&_windowData.WindowClassInformation);
        _windowData.WindowClassInformation = {};

        _initialized = false;
    }

    void    Initialize();
    void    SetFrameFlag(std::initializer_list<WindowFrameFlag> flags) { Utils::CombineFlag(_windowData.FrameFlag, flags); }

    void    _Native_UpdateWindowSize(Size);
    void    _Native_UpdateWindowPosition(Point);
    LRESULT _Native_ComponentMessageProcessor(UINT, WPARAM, LPARAM, bool&);

  private:
    void _UpdateSwapBufferSize() {
        if (not _swapBuffer) {
            return;
        }

        static Size lastSize{WindowSize};

        if (lastSize.Equals(WindowSize)) {
            return;
        }

        _swapBuffer->RefreshSize();
        lastSize = WindowSize;
    }

  public:
    static unordered_map<HWND, Window*>& GetWindowMap() { return WindowMap; }

    unique_ptr<Logic::CComponentTree>    componentTree; // 组件树类，有命中测试的方法

  private:
    bool                                _initialized{false};
    unique_ptr<Render::SwapBuffer>      _swapBuffer;

    static unordered_map<HWND, Window*> WindowMap;

    /*
     *
     *    Component Data Part.
     *    Window data:
     *        Type --- Property -------- Access ------- Comment -------------------------------
     *        HWND     WindowHandle      Getter Only      /
     *        wstring  WindowTitleText   Getter/Setter    /
     *        Size     WindowSize        Getter/Setter  Do not be called by Native. !!!!!!!!!!!
     *        Point    WindowPosition    Getter/Setter  Do not be called by Native. !!!!!!!!!!!
     *
     *     Base Data: (Rewrited)
     *        Type - Property --------- Access ----- Comment -----------------------------------------------------------------------------------------------------
     *        Point  ComponentPosition  Getter Only  Position of component could not be changed. If want to move window, you should change WindowPosition instead.
     *
     */

    WindowDataType _windowData{};

  public:
    const WindowDataType& GetWindowData() const { return _windowData; }
    void                  SetWindowData(_In_ const WindowDataType& data) { _windowData = data; }

    const UINT&           GetFrameStatus() const { return _windowData.FrameFlag; }

    const HWND            GetWindowHandle() const { return _windowData.WindowHandle; }

    const wstring&        GetWindowClassText() const { return _windowData.WindowClassText; }
    void                  SetWindowClassText(const wstring& classText) {
        if (_initialized) {
            throw std::logic_error("Window class cannot be changed after having been initialized. ");
        }

        _windowData.WindowClassText = classText;
    }

    const wstring& GetWindowTitle() const { return _windowData.WindowTitleText; }
    void           SetWindowTitle(const wstring& titleText) {
        if (_initialized) {
            ::SetWindowTextW(WindowHandle, titleText.c_str());
        }

        _windowData.WindowTitleText = titleText;
    }

    // DELETE ME // SetWindowSize/Position -> WindowsMessageProcessor -> _Native_SetWindowSize/Position.

    Size GetWindowSize() const { return _windowData.WindowSize; }
    void SetWindowSize(_In_ const Size& size) {
        if (_initialized) {
            ::SetWindowPos(WindowHandle, nullptr, 0, 0, size.Width, size.Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
        _windowData.WindowSize = size;
    }

    Point GetWindowPosition() const { return _windowData.WindowPosition; }
    void  SetWindowPosition(_In_ const Point& pos) {
        if (_initialized) {
            ::SetWindowPos(WindowHandle, nullptr, pos.X, pos.Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }

        _windowData.WindowPosition = pos;
    }

  public:
    COMPONENT_PROPERTY(GetWindowData, SetWindowData) WindowDataType    WindowData;
    COMPONENT_PROPERTY_GETTER_ONLY(GetFrameStatus) UINT                FrameFlags;

    COMPONENT_PROPERTY_GETTER_ONLY(GetWindowHandle) HWND               WindowHandle;
    COMPONENT_PROPERTY(GetWindowClassText, SetWindowClassText) wstring WindowClassText;
    COMPONENT_PROPERTY(GetWindowTitle, SetWindowTitle) wstring         WindowTitle;
    COMPONENT_PROPERTY(GetWindowSize, SetWindowSize) Size              WindowSize;
    COMPONENT_PROPERTY(GetWindowPosition, SetWindowPosition) Point     WindowPosition;

    COMPONENT_PROPERTY_GETTER_ONLY(GetComponentPosition) Point         ComponentPosition;
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
// class UIENGINE_API Window : public BaseComponent {
// public:
//     Window(vector<Utils::PropertyPair>);
//     ~Window() {
//         DestroyWindow(_windowHandle);
//         RegisterClassExW(&_windowClassInfo);
//
//         _windowClassInfo = {};
//     }
//
// public:
//     unique_ptr<Logic::CComponentTree>& GetComponentTree() { return _componentTree; };
//     // User musn't change the position via using this method. Or？
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
//     void SetWindowOwner(Window* pOwner) {
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
//         static unordered_map<Window*, Size> lastSizeMap{};
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
