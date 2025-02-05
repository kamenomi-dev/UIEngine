#pragma once

#ifndef __ENGINE_COMPONENT_H__
#define __ENGINE_COMPONENT_H__

namespace Engine {
/*
namespace Render {
class UIENGINE_API IRenderable : Interface::IUninheritable {
public:
    IRenderable();
    ~IRenderable();

    virtual void Render();
};
} // namespace Render

namespace Component {
namespace Defines {
UIENGINE_API typedef struct __structWindowProps {
    bool           disabled;
    bool           visibled;
    const wchar_t* titleText;
    const wchar_t* className;
    Rect           wndRect;

    __structWindowProps();
    __structWindowProps(const __structWindowProps&);
} IWindowProps;

UIENGINE_API typedef struct __structWindowData : public IWindowProps {
    bool isMainWindow;
    HWND hWnd;
    HWND hParentWindow;

    __structWindowData();
    __structWindowData(const __structWindowData&);
    __structWindowData(HWND, __structWindowProps props);
} WindowData;
} // namespace Defines

using namespace Defines;

class UIENGINE_API CWindow : Interface::IUninheritable {
public:
    CWindow(HWND, IWindowProps);
    ~CWindow();

    bool       IsMainWindow() const;
    WindowData GetWindowData() const;
    void       SetAsMainWindow();

private:
    WindowData* _windowData;

    HWND __InitializeWindow();
};
} // namespace Component
*/
} // namespace Engine

#endif //__ENGINE_COMPONENT_H__