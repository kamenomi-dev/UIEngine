/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

class UIENGINE_API UIManager final {
  public:
    void           InitializeWindow(vector<Components::Window*>);
    static LRESULT WindowsMessageProcessor(HWND, UINT, WPARAM, LPARAM);

  private:
    vector<Components::Window*> _windowList;

    // Single instance.

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

    ~UIManager()                           = default;
    UIManager(const UIManager&)            = delete;
    UIManager& operator=(const UIManager&) = delete;

  private:
    static unique_ptr<UIManager> _selfInstance;

    friend class unique_ptr<UIManager>;
    UIManager(HINSTANCE hInstance) {
        if (_selfInstance) {
            throw runtime_error("Error! UIManager has initialized already! ");
        }
    };
};

} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
