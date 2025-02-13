#include "pch.h"

#include "../engine.h"

using namespace Engine::Component;

// Component Message

void CBase::Render(Gdiplus::Graphics& graphics) {
    static unordered_map<void*, Gdiplus::Color> map{};

    if (map.find(this) == map.end()) {
        map[this] = Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
    }

    graphics.Clear(map[this]);
}

LRESULT CBase::_Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn) {
    bIsReturn = false;
    return NULL;
}

void CBase::_Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == CM_MOUSE_HOVER) {
        (void)cos(1);// FIXME: ???
    }

    if (uMsg == CM_MOUSE_LEAVE) {
        (void)cos(1);
    }

    if (uMsg == CM_PAINT) {
        auto& graphics = *(Gdiplus::Graphics*)lParam;

        const auto lastState = graphics.Save();
        {
            graphics.SetClip(GetComponentRect());
            Render(graphics);
        }
        graphics.Restore(lastState);
    }
}
