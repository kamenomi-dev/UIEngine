/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

class UIENGINE_API UIManager final {
public:
    static UIManager* Initialize(HINSTANCE hInstance);
    static UIManager* Get();

    ~UIManager();

    HINSTANCE GetProcessInstance() const;

    Component::CWindow* CreateCentralWindow(
        wstring             titleText,
        wstring             className    = L"UIEngine.Window",
        Size                windowSize   = {800, 600},
        Component::CWindow* parentWindow = nullptr
    );
    Component::CWindow* CreateNormalWindow(
        wstring             titleText,
        wstring             className    = L"UIEngine.Window",
        Rect                windowRect   = {0, 0, 800, 600},
        Component::CWindow* parentWindow = nullptr
    );

    static LRESULT WindowsMessageProcessor(HWND, UINT, WPARAM, LPARAM);
    WPARAM         StartMessageLoop();

private:
    static UIManager*                         __hInstance;

    HINSTANCE                                 _hProcessInstance;
    unordered_map<HWND, Component::CWindow*>* _windowMap;// TODO: ¸ÄÎªÆÕÍ¨×Ö¶Î
    unordered_map<HWND, Component::CWindow*>* _mainWindowMap;

    UIManager(HINSTANCE hInstance);

    void __InsertWindowMap(HWND, Component::CWindow*);
};

} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
