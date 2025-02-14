/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine::Component {

class UIENGINE_API CWindow : public CBase {
public:
    CWindow(vector<Utils::PropertyPair>);
    ~CWindow() {
        DestroyWindow(_windowHandle);
        RegisterClassExW(&_windowClassInfo);

        _windowClassInfo = {};
    }

public:
    unique_ptr<Logic::CComponentTree>& GetComponentTree() { return _componentTree; };
    // User musn't change the position via using this method. Or£¿
    Point _Property_SetComponentPosition() const = delete;

    virtual wstring _Property_GetComponentClass() const { return L"Component.Window"s; }

    bool IsOwnerWindow() const { return GetPropertyTyped<bool>(L"isOwnerWindow"); }
    HWND GetWindowHandle() const {
        if (!_windowHandle) {
            throw runtime_error("Window hasn't created yet! ");
        }

        return _windowHandle;
    }
    void SetWindowOwner(CWindow* pOwner) {
        SetPropertyByValue(L"parentWindow", pOwner);
        SetPropertyByValue(L"isOwnerWindow", pOwner != nullptr);
    }


    Render::SwapBuffer& GetWindowSwapBuffer() { return *_renderSwapBuffer; }

    void Render(Gdiplus::Graphics&);

    LRESULT
    _Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);

    void _Native_SetWindowSize(Size newSize) {

        ComponentSize      = newSize;
        _windowRect.Width  = newSize.Width;
        _windowRect.Height = newSize.Height;

        _UpdateWindowSwapBuffer();
    }

    void _Native_SetWindowPosition(Point newPosition) {
        _windowRect.X = newPosition.X;
        _windowRect.Y = newPosition.Y;

        _UpdateWindowSwapBuffer();
    }

private:
    HWND                              _windowHandle;
    WNDCLASSEXW                       _windowClassInfo;
    unique_ptr<Logic::CComponentTree> _componentTree;
    unique_ptr<Render::SwapBuffer>    _renderSwapBuffer;

    void _RegisterClass();
    HWND _CreateWindow();
    void _UpdateWindowSwapBuffer() {
        static unordered_map<CWindow*, Size> lastSizeMap{};
        auto&                                lastSize = lastSizeMap[this];

        if (not lastSize.Equals(ComponentSize)) {
            _renderSwapBuffer->RefreshSize();

            lastSize = ComponentSize;
        }
    }

private:
    // Property

    Rect _windowRect{};

public:
    COMPONENT_PROPERTY(_Property_GetWindowRect, _Property_SetWindowRect) Rect WindowRect;
    COMPONENT_PROPERTY(_Property_GetWindowSize, _Property_SetWindowSize) Size WindowSize;
    COMPONENT_PROPERTY(_Property_GetWindowPosition, _Property_SetWindowPosition) Point WindowPosition;

    Rect& _Property_GetWindowRect() { return _windowRect; }
    void  _Property_SetWindowRect(Rect newRect) {
        WindowSize     = Size{newRect.Width, newRect.Height};
        WindowPosition = Point{newRect.X, newRect.Y};
    }

    Size _Property_GetWindowSize() const { return {_windowRect.Width, _windowRect.Height}; }
    void _Property_SetWindowSize(Size newSize) {
        ComponentSize      = newSize;
        _windowRect.Width  = newSize.Width;
        _windowRect.Height = newSize.Height;

        _UpdateWindowSwapBuffer();

        SetWindowPos(
            _windowHandle,
            nullptr,
            _windowRect.X,
            _windowRect.Y,
            _windowRect.Width,
            _windowRect.Height,
            SWP_NOOWNERZORDER
        );
    }

    Point _Property_GetWindowPosition() const { return {_windowRect.X, _windowRect.Y}; }
    void  _Property_SetWindowPosition(Point newPosition) {
        _windowRect.X = newPosition.X;
        _windowRect.Y = newPosition.Y;

        _UpdateWindowSwapBuffer();

        SetWindowPos(
            _windowHandle,
            nullptr,
            _windowRect.X,
            _windowRect.Y,
            _windowRect.Width,
            _windowRect.Height,
            SWP_NOOWNERZORDER
        );
    }
};
} // namespace Engine::Component

#endif // !__COMPONENT_WINDOW_H__
