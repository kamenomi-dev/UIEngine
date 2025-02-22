/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_LOGIC_H__
#define __ENGINE_LOGIC_H__

namespace Engine {

namespace Component {
class Window;
class ComponentBase;
} // namespace Component

namespace Logic {

namespace Defines {

typedef Component::ComponentBase*(TTryHittestCondition)(LPVOID,
                                                Component::ComponentBase*,
                                                Component::ComponentBase*,
                                                vector<Component::ComponentBase*>&);
typedef void(TTryHittestConditionNext)(LPVOID, TTryHittestCondition, vector<Component::ComponentBase*>&, vector<Component::ComponentBase*>&);

} // namespace Defines

using namespace Defines;

class UIENGINE_API CComponentTree final {
public:
    CComponentTree(Component::Window* pRootWindow) : _pRootWindow(pRootWindow) {}

    Component::Window*       GetRootWindow() const { return _pRootWindow; };
    vector<Component::ComponentBase*> TryHitTest(Rect);
    vector<Component::ComponentBase*> TryHitTest(Point);
    vector<Component::ComponentBase*> TryHitTestWithCondition(TTryHittestCondition, LPVOID lParam);

private:
    Component::Window* _pRootWindow{nullptr};
};
} // namespace Logic
} // namespace Engine

#endif //__ENGINE_LOGIC_H__