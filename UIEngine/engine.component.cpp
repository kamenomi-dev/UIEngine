#include "pch.h"
#include "engine.h"

using namespace Engine::Components;

static std::stack<Component*> component{};

// -

void Engine::Begin(const wstring& title) {
    static Window* lastWindow = nullptr;
    if (lastWindow == component.top()) {
        throw std::logic_error("Cannot begin a window repeatedly. ");
    }

    if (!component.empty()) {
        if (component.top()->GetComponentID() == L"Component.Window") {
            throw std::logic_error("Cannot begin a new window as last window hasn't ended yet. ");
        }
    }

    auto window = new Window{};
    window->WindowTitle = title;
    component.push(window);
}

void Engine::End() { Component* currentComponent = component.top();
    while (currentComponent) {

      if (!component.empty()) {
            component.pop();
            currentComponent = component.top();
        }
    }
}
