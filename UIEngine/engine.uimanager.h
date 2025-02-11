/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

class UIENGINE_API UIManager final {
public:
    [[nodiscard]] inline HINSTANCE GetProcessInstance() const { return _hProcessInstance; };
    [[nodiscard]] inline auto&     GetWindowMap() { return _windowMap; };

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
    inline int     StartMessageLoop() {
        MSG msgStruct{};

        while (GetMessage(&msgStruct, nullptr, 0, 0)) {
            TranslateMessage(&msgStruct);
            DispatchMessage(&msgStruct);
        }

        return (int)msgStruct.wParam; // Todo, does it will lost?
    };

private:
    HINSTANCE                                _hProcessInstance{NULL};
    unordered_map<HWND, Component::CWindow*> _windowMap{};
    unordered_map<HWND, Component::CWindow*> _mainWindowMap{};

    inline void __InsertWindowMap(HWND, Component::CWindow*);


public:
    inline static auto& Initialize(HINSTANCE hInstance) {
        if (__hInstance) {
            throw runtime_error("Error! UIManager has initialized already! ");
        }

        return __hInstance = unique_ptr<UIManager>(new UIManager(hInstance));
    };
    inline static auto& Get() {
        if (!__hInstance) {
            throw runtime_error("Error! UIManager hasn't initialized yet! ");
        }

        return *__hInstance;
    };

    ~UIManager() { Engine::Uninitialize(); };
    UIManager(const UIManager&)            = delete;
    UIManager& operator=(const UIManager&) = delete;

private:
    static unique_ptr<UIManager> __hInstance;

    friend class unique_ptr<UIManager>;
    UIManager(HINSTANCE hInstance) {
        if (__hInstance) {
            throw runtime_error("Error! UIManager has initialized already! ");
        }

        _hProcessInstance = hInstance;
        Engine::Initialize(hInstance);
    };
};

} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
