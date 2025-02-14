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

LRESULT CBase::_Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& isReturn) {

    if (uMsg == WM_CLOSE) {
        _Native_TransformMessageProcessor(CM_DESTROY, NULL, NULL);

        for (auto& child : GetChildCompnents()) {
            child->_Native_ComponentMessageProcessor(WM_CLOSE, NULL, NULL, isReturn);

            delete child;
            child = nullptr;
        }

        GetChildCompnents().clear();
    }

    return 0;
}

void CBase::_Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == CM_DESTROY) {
        (void)cos(1);
    }

    if (uMsg == CM_MOUSE_HOVER) {
        (void)cos(1); // FIXME: ???
    }

    if (uMsg == CM_MOUSE_LEAVE) {
        (void)cos(1);
    }

    if (uMsg == CM_PAINT) {
        Render(*(Gdiplus::Graphics*)lParam);
    }
}
