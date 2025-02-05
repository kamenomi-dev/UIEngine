#pragma once

#ifndef __COMPONENT_WINDOW_H__
#define __COMPONENT_WINDOW_H__

namespace Engine {
namespace Component {

class UIENGINE_API CWindow : public CBase {
public:
    CWindow(vector<PropertyPair>);
    ~CWindow();

public:
    wstring GetComponentClass() const;
    Point   SetComponentPosition() const = delete; // don't do that.

    bool  IsOwnerWindow() const;
    HWND  GetWindowHandle() const;
    void  SetWindowOwner(CWindow*);
    void  SetWindowSize(Size, bool isNative = false);
    void  SetWindowPosition(Point, bool isNative = false);
    Size  GetWindowSize() const;
    Point GetWindowPosition() const;

    // Bad using.
    Render::CSwapBuffer* GetWindowSwapBuffer();

    virtual void Render(Gdiplus::Graphics&);

    LRESULT __Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);

private:
    HWND                 _hSelfWindow;
    WNDCLASSEXW          _wndClassInfo;
    Render::CSwapBuffer* _renderSwapBuffer;

    void __RegisterClass();
    void __UnregisterClass();
    void __UpdateRectangle(bool isNative = false);
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_WINDOW_H__
