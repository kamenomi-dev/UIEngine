/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

// REVIEW: UIManager是一个空壳类，是否需要移除？
class UIENGINE_API UIManager final {
  public:
    void           InitializeWindow(vector<Components::Window*>);
    static LRESULT WindowsMessageProcessor(HWND, UINT, WPARAM, LPARAM);

  private:
    vector<Components::Window*> _windowList;

    // Single instance.

  public:
    static auto& Get() {
        static UIManager instance{};
        return instance;
    };

    UIManager() = default;
    UIManager(const UIManager&)            = delete;
    UIManager& operator=(const UIManager&) = delete;
};

} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
