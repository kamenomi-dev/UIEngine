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
using namespace Engine::Components;

// Compenont Tree - -  Hittest Implement

static void __TryHitTestConditionNext(LPVOID lParam, TTryHittestCondition&& conditionFunc, Component* currComp, vector<Component*>& resultComponents) {

    auto current = currComp;

    while (current != nullptr) {

        auto resultComp = conditionFunc(lParam, currComp, currComp->NodeData.FirstChild, resultComponents);

        if (resultComp != nullptr) {
            resultComponents.push_back(resultComp);
        }

        current = current->NodeData.Next;
    }
};

static Component* __TryHitTestFromRect(LPVOID pTargetRect, Component* currComp, Component* childCompBegin, vector<Component*>& resultComponents) {
    const auto& targetRect  = *((Rect*)pTargetRect);
    const auto& currentRect = Rect(currComp->ComponentPosition, currComp->ComponentSize);

    if (currentRect.IntersectsWith(targetRect)) {

        __TryHitTestConditionNext(pTargetRect, __TryHitTestFromRect, childCompBegin, resultComponents);
        return currComp;
    }

    return nullptr;
}

static Component* __TryHitTestFromPoint(LPVOID pTargetPoint, Component* currComp, Component* childCompBegin, vector<Component*>& resultComponents) {
    const auto& targetPoint = *(Point*)pTargetPoint;
    const auto& currentRect = Rect(currComp->ComponentPosition, currComp->ComponentSize);

    if (currentRect.Contains(targetPoint)) {

        __TryHitTestConditionNext(pTargetPoint, __TryHitTestFromPoint, childCompBegin, resultComponents);
        return currComp;
    }

    return nullptr;
}

vector<Component*> CComponentTree::TryHitTest(Rect targetRect) { return TryHitTestWithCondition(__TryHitTestFromRect, &targetRect); }

vector<Component*> CComponentTree::TryHitTest(Point targetPoint) { return TryHitTestWithCondition(__TryHitTestFromPoint, &targetPoint); }

vector<Component*> CComponentTree::TryHitTestWithCondition(TTryHittestCondition conditionFunc, LPVOID lParam) {
    vector<Component*> resultComps{};

    resultComps.push_back(conditionFunc(lParam, _pRootWindow, _pRootWindow->NodeData.FirstChild, resultComps));

    return resultComps;
}
