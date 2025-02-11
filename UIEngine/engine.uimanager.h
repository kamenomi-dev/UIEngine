/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

class UIENGINE_API UIManager final {
public:

  friend class unique_ptr<UIManager>;

    inline static auto& Initialize(HINSTANCE hInstance) {
        if (__hInstance) {
            return __hInstance;
        }

        return __hInstance = make_unique<UIManager>(hInstance);
    };

    inline static auto& Get() {
        CHECK_RESULT_BOOL(__hInstance);
        return __hInstance;
    };

    ~UIManager() { Engine::Uninitialize(); };

    HINSTANCE GetProcessInstance() const { return _hProcessInstance; };

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
    inline WPARAM  StartMessageLoop();

private:
    static unique_ptr<UIManager> __hInstance;

    UIManager(HINSTANCE hInstance) {
        if (__hInstance) {
            DebugBreak();
            abort();
            return;
        }

        _hProcessInstance = hInstance;
        Engine::Initialize(hInstance);
    };

    HINSTANCE                                _hProcessInstance;
    unordered_map<HWND, Component::CWindow*> _windowMap{};
    unordered_map<HWND, Component::CWindow*> _mainWindowMap{};

    inline void __InsertWindowMap(HWND, Component::CWindow*);
};

} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
