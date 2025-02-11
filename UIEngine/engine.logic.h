/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_LOGIC_H__
#define __ENGINE_LOGIC_H__

namespace Engine {

namespace Component {
class CWindow;
class CBase;
} // namespace Component

namespace Logic {

namespace Defines {

typedef Component::CBase*(TTryHittestCondition)(LPVOID,
                                                Component::CBase*,
                                                vector<Component::CBase*>&,
                                                vector<Component::CBase*>&);
typedef void(TTryHittestConditionNext)(LPVOID, TTryHittestCondition, vector<Component::CBase*>&, vector<Component::CBase*>&);

} // namespace Defines

using namespace Defines;

class UIENGINE_API CComponentTree final {
public:
    CComponentTree(Component::CWindow* pRootWindow) : _pRootWindow(pRootWindow) {}

    Component::CWindow*       GetRootWindow() const { return _pRootWindow; };
    vector<Component::CBase*> TryHitTest(Rect);
    vector<Component::CBase*> TryHitTest(Point);
    vector<Component::CBase*> TryHitTestWithCondition(TTryHittestCondition, LPVOID lParam);

private:
    Component::CWindow* _pRootWindow{nullptr};
};
} // namespace Logic
} // namespace Engine

#endif //__ENGINE_LOGIC_H__