/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine::Components {

class Window;
  
struct UIENGINE_API WindowInfoType {
    Window*    ThisWindow;
    Component* LastComponent;
};

struct UIENGINE_API WindowDataType {
    UINT       FrameFlag{(UINT)WndFrame::None};

    HWND       WindowHandle{nullptr};
    bool       IsOwnerWindow{false}; // Owner window is unique to all of windows.
    Size       WindowSize{800, 600};
    Point      WindowPosition{0, 0};
    wstring    WindowClassText{L"Component_Window"};
    wstring    WindowTitleText{L"视窗"};
    WNDCLASSEX WindowClassInformation{};
};

class UIENGINE_API Window : public Component {
  public:
    Window() : Component() {
        _componentData.ComponentSize     = {800, 600};
        _componentData.ComponentPosition = {0, 0};
    }
    ~Window() {
        if (!_initialized) {
            OutputDebugStringW(L"Window class was incorrectly initialized again. ");
            abort();
        }

        DestroyWindow(WindowHandle);

        RegisterClassEx(&_windowData.WindowClassInformation);
        _windowData.WindowClassInformation = {};

        _initialized = false;
    }

    void    Initialize();
    void    SetFrameFlag(WndFrame flags) { Utils::Flags::CombineFlag(_windowData.FrameFlag, {flags}); }

    void    _Native_UpdateWindowSize(Size);
    void    _Native_UpdateWindowPosition(Point);
    LRESULT _Native_ComponentMessageProcessor(UINT, WPARAM, LPARAM, bool&);

  private:
    void _UpdateSwapBufferSize() {
        static Size lastSize{WindowSize};

        if (not swapBuffer) {
            return;
        }

        if (lastSize.Equals(WindowSize)) {
            return;
        }

        swapBuffer->RefreshSize();
        lastSize = WindowSize;
    }

  public:
    static unordered_map<HWND, WindowInfoType>& GetWindowMap() { return windowMap; }

    unique_ptr<Utils::Graph::SwapBuffer> swapBuffer;
    unique_ptr<Logic::ComponentTree>     componentTree;

  private:
    bool                                _initialized{false};

    static unordered_map<HWND, WindowInfoType> windowMap;

    public:

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

} // namespace Engine::Components

#endif // !__COMPONENT_WINDOW_H__
