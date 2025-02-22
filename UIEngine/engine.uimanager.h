/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

class UIENGINE_API UIManager final {
public:
    [[nodiscard]] HINSTANCE GetProcessInstance() const { return _processInstance; };

    void InitializeWindow(vector<Component::Window*>);

    static LRESULT WindowsMessageProcessor(HWND, UINT, WPARAM, LPARAM);
    int            StartMessageLoop() {
        MSG msgStruct{};

        while (GetMessageW(&msgStruct, nullptr, 0, 0)) {
            TranslateMessage(&msgStruct);
            DispatchMessageW(&msgStruct);
        }

        return (int)msgStruct.wParam; // Todo, does it will lost?
    };

private:
    HINSTANCE                   _processInstance{};
    vector<Component::Window*> _windowList;

public:
    static auto& Initialize(HINSTANCE instance) {
        if (_selfInstance) {
            throw runtime_error("Error! UIManager has initialized already! ");
        }

        return _selfInstance = unique_ptr<UIManager>(new UIManager(instance));
    };
    static auto& Get() {
        if (!_selfInstance) {
            throw runtime_error("Error! UIManager hasn't initialized yet! ");
        }

        return *_selfInstance;
    };

    ~UIManager() { Engine::Uninitialize(); };
    UIManager(const UIManager&)            = delete;
    UIManager& operator=(const UIManager&) = delete;

private:
    static unique_ptr<UIManager> _selfInstance;

    friend class unique_ptr<UIManager>;
    UIManager(HINSTANCE hInstance) {
        if (_selfInstance) {
            throw runtime_error("Error! UIManager has initialized already! ");
        }

        _processInstance = hInstance;
        Engine::Initialize(hInstance);
    };
};

} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
