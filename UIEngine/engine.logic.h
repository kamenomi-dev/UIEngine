/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_LOGIC_H__
#define __ENGINE_LOGIC_H__

namespace Engine {

namespace Components {
class Window;
class Component;
} // namespace Component

namespace Logic {

namespace Defines {

typedef Components::Component*(TTryHittestCondition)(LPVOID, Components::Component*, Components::Component*, vector<Components::Component*>&);
typedef void(TTryHittestConditionNext)(LPVOID, TTryHittestCondition, vector<Components::Component*>&, vector<Components::Component*>&);

} // namespace Defines

using namespace Defines;

class UIENGINE_API CComponentTree final {
public:
    CComponentTree(Components::Window* pRootWindow) : _pRootWindow(pRootWindow) {}

    Components::Window*            GetRootWindow() const { return _pRootWindow; };
    vector<Components::Component*> TryHitTest(Rect);
    vector<Components::Component*> TryHitTest(Point);
    vector<Components::Component*> TryHitTestWithCondition(TTryHittestCondition, LPVOID lParam);

private:
    Components::Window* _pRootWindow{nullptr};
};
} // namespace Logic
} // namespace Engine

#endif //__ENGINE_LOGIC_H__