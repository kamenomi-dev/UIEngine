/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine {
namespace Component {

class UIENGINE_API CWindow : public CBase {
public:
    CWindow(vector<Utils::PropertyPair>);
    ~CWindow();

public:
    unique_ptr<Logic::CComponentTree>& GetComponentTree();

    wstring GetComponentClass() const;
    Point   SetComponentPosition() const = delete; // don't do that.

    bool         IsOwnerWindow() const;
    HWND         GetWindowHandle() const;
    void         SetWindowOwner(CWindow*);
    inline void  SetWindowSize(Size);
    inline void  SetWindowPosition(Point);
    inline Size  GetWindowSize() const;
    inline Point GetWindowPosition() const;

    unique_ptr<Render::SwapBuffer>& GetWindowSwapBuffer();

    void        Render(Gdiplus::Graphics&);
    inline void __Native_SetWindowSize(Size);
    inline void __Native_SetWindowPosition(Point);
    inline LRESULT
    __Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);

private:
    HWND                              _hSelfWindow;
    WNDCLASSEXW                       _wndClassInfo;
    unique_ptr<Logic::CComponentTree> _componentTree;
    unique_ptr<Render::SwapBuffer>    _renderSwapBuffer;

    void __RegisterClass();
    void __UnregisterClass();
    HWND __CreateWindow();
    void __DeleteWindow();
    void __UpdateRectangle();
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_WINDOW_H__
