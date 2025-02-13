/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine {
namespace Component {

class UIENGINE_API CWindow : public CBase {
public:
    CWindow(vector<Utils::PropertyPair>);
    ~CWindow() {
        __DeleteWindow();
        __UnregisterClass();
    }

public:
    unique_ptr<Logic::CComponentTree>& GetComponentTree() { return _componentTree; };
    inline wstring                     GetComponentClass() const { return L"Window"s; }
    Point                              SetComponentPosition() const = delete; // don't do that.

    inline bool IsOwnerWindow() const { return GetPropertyTyped<bool>(L"isOwnerWindow"); }
    inline HWND GetWindowHandle() const {
        if (!_hSelfWindow) {
            throw runtime_error("Window hasn't created yet! ");
        }

        return _hSelfWindow;
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
            _hSelfWindow,
            nullptr,
            windowRect.X,
            windowRect.Y,
            windowRect.Width,
            windowRect.Height,
            SWP_NOOWNERZORDER
        );
        __UpdateRectangle();
    }
    void SetWindowPosition(Point newPosition) {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

        windowRect.X = newPosition.X;
        windowRect.Y = newPosition.Y;

        SetWindowPos(
            _hSelfWindow,
            nullptr,
            windowRect.X,
            windowRect.Y,
            windowRect.Width,
            windowRect.Height,
            SWP_NOOWNERZORDER
        );
        __UpdateRectangle();
    }
    inline Size GetWindowSize() const {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");
        Size  size{};

        windowRect.GetSize(&size);

        return size;
    }
    inline Point GetWindowPosition() const {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");
        Point position{};

        windowRect.GetLocation(&position);

        return position;
    }

    Render::SwapBuffer& GetWindowSwapBuffer() { return *_renderSwapBuffer; }

    void        Render(Gdiplus::Graphics&);
    inline void _Native_SetWindowSize(Size newSize) {
        auto& windowRect    = GetPropertyTyped<Rect>(L"windowRect");
        auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

        windowRect.Width = componentRect.Width = newSize.Width;
        windowRect.Height = componentRect.Height = newSize.Height;

        __UpdateRectangle();
    }
    inline void _Native_SetWindowPosition(Point newPosition) {
        auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

        windowRect.X = newPosition.X;
        windowRect.Y = newPosition.Y;

        __UpdateRectangle();
    }

    inline LRESULT
    _Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);

private:
    HWND                              _hSelfWindow;
    WNDCLASSEXW                       _wndClassInfo;
    unique_ptr<Logic::CComponentTree> _componentTree;
    unique_ptr<Render::SwapBuffer>    _renderSwapBuffer;

    inline void __RegisterClass();
    inline void __UnregisterClass() { (RegisterClassExW(&_wndClassInfo)); }
    inline HWND __CreateWindow();
    inline void __DeleteWindow() { DestroyWindow(_hSelfWindow); }
    inline void __UpdateRectangle() {
        static Rect lastRect{};
        const auto& currRect = GetPropertyTyped<Rect>(L"componentRect");

        if (not lastRect.Equals(currRect)) {

            if (lastRect.Width != currRect.Width || lastRect.Height != currRect.Height) {
                _renderSwapBuffer->RefreshSize();
            }

            lastRect = currRect;
        }
    }
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_WINDOW_H__
