/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
*    Gdiplus UI, using gdiplus, is a UI library of Windows platform which
*    is based on C++.
*     Copyright (C) 2025  Project Contributors
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*     any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "pch.h"

#include "engine.h"

using namespace Engine::Logic;
using namespace Engine::Component;

// Compenont Tree - -  Hittest Implement

inline static void __TryHitTestConditionNext(
    LPVOID                 lParam,
    TTryHittestCondition&& conditionFunc,
    vector<CBase*>&        components,
    vector<CBase*>&        resultComponents
) {
    for (size_t idx = 0; idx < components.size(); idx++) {
        auto currComp = components[idx];
        if (currComp == nullptr) {
            continue;
        }

        auto resultComp = conditionFunc(lParam, currComp, currComp->GetChildCompnents(), resultComponents);

        if (resultComp != nullptr) {
            resultComponents.push_back(resultComp);
        }
    }
};

inline static CBase* __TryHitTestFromRect(
    LPVOID          pTargetRect,
    CBase*          currComp,
    vector<CBase*>& currCompChildren,
    vector<CBase*>& resultComponents
) {
    const auto& targetRect  = *((Rect*)pTargetRect);
    const auto& currentRect = currComp->GetPropertyTyped<Rect>(L"componentRect");

    if (currentRect.IntersectsWith(targetRect)) {

        __TryHitTestConditionNext(pTargetRect, __TryHitTestFromRect, currCompChildren, resultComponents);
        return currComp;
    }

    return nullptr;
}

inline static CBase* __TryHitTestFromPoint(
    LPVOID          pTargetPoint,
    CBase*          currComp,
    vector<CBase*>& currCompChildren,
    vector<CBase*>& resultComponents
) {
    const auto& targetPoint = *(Point*)pTargetPoint;
    const auto& currentRect = currComp->GetPropertyTyped<Rect>(L"componentRect");

    if (currentRect.Contains(targetPoint)) {

        __TryHitTestConditionNext(pTargetPoint, __TryHitTestFromPoint, currCompChildren, resultComponents);
        return currComp;
    }

    return nullptr;
}

vector<CBase*> CComponentTree::TryHitTest(Rect targetRect) {
    return TryHitTestWithCondition(__TryHitTestFromRect, &targetRect);
}

vector<CBase*> CComponentTree::TryHitTest(Point targetPoint) {
    return TryHitTestWithCondition(__TryHitTestFromPoint, &targetPoint);
}

vector<CBase*> CComponentTree::TryHitTestWithCondition(TTryHittestCondition conditionFunc, LPVOID lParam) {
    vector<CBase*> resultComps{};

    resultComps.push_back(conditionFunc(lParam, _pRootWindow, _pRootWindow->GetChildCompnents(), resultComps));

    return resultComps;
}
