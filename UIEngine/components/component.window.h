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
    wstring                            GetComponentClass() const { return L"Window"s; }
    Point                              SetComponentPosition() const = delete; // don't do that.

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
    void SetWindowSize(Size newSize) {
        auto& windowRect    = GetPropertyTyped<Rect>(L"windowRect");
        auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

        windowRect.Width = componentRect.Width = newSize.Width;
        windowRect.Height = componentRect.Height = newSize.Height;

        SetWindowPos(
            _windowHandle,
            nullptr,
            windowRect.X,
            windowRect.Y,
            windowRect.Width,
            windowRect.Height,
            SWP_NOOWNERZORDER
        );
        _UpdateWindowSwapBuffer();
    }
    void SetWindowPosition(Point newPosition) {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

        windowRect.X = newPosition.X;
        windowRect.Y = newPosition.Y;

        SetWindowPos(
            _windowHandle,
            nullptr,
            windowRect.X,
            windowRect.Y,
            windowRect.Width,
            windowRect.Height,
            SWP_NOOWNERZORDER
        );
        _UpdateWindowSwapBuffer();
    }
    Size GetWindowSize() const {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");
        Size  size{};

        windowRect.GetSize(&size);

        return size;
    }
    Point GetWindowPosition() const {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");
        Point position{};

        windowRect.GetLocation(&position);

        return position;
    }

    Render::SwapBuffer& GetWindowSwapBuffer() { return *_renderSwapBuffer; }

    void Render(Gdiplus::Graphics&);
    void _Native_SetWindowSize(Size newSize) {
        auto& windowRect    = GetPropertyTyped<Rect>(L"windowRect");
        auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

        windowRect.Width = componentRect.Width = newSize.Width;
        windowRect.Height = componentRect.Height = newSize.Height;

        _UpdateWindowSwapBuffer();
    }
    void _Native_SetWindowPosition(Point newPosition) {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

        windowRect.X = newPosition.X;
        windowRect.Y = newPosition.Y;

        _UpdateWindowSwapBuffer();
    }

    LRESULT
    _Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);

private:
    HWND                              _windowHandle;
    WNDCLASSEXW                       _windowClassInfo;
    unique_ptr<Logic::CComponentTree> _componentTree;
    unique_ptr<Render::SwapBuffer>    _renderSwapBuffer;

    void _RegisterClass();
    HWND _CreateWindow();
    void _UpdateWindowSwapBuffer() {
        static unordered_map<CWindow*, Size> lastSizeMap{};

        auto&       lastSize    = lastSizeMap[this];
        const auto& currentSize = GetComponentSize();

        if (not lastSize.Equals(currentSize)) {
            _renderSwapBuffer->RefreshSize();

            lastSize = currentSize;
        }
    }
};
} // namespace Engine::Component

#endif // !__COMPONENT_WINDOW_H__
