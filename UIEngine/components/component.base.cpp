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
#include "../engine.h"

using namespace Engine::Component;

// ComponentBase Message

void ComponentBase::Render(Gdiplus::Graphics& graphics) {
    static unordered_map<void*, Gdiplus::Color> map{};

    if (map.find(this) == map.end()) {
        map[this] = Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
    }

    graphics.Clear(map[this]);
}

LRESULT ComponentBase::_Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& isReturn) {

    if (uMsg == WM_CLOSE) {

        auto current = NodeData.FirstChild;
        while (current != nullptr) {
            current->_Native_TransformMessageProcessor(CM_DESTROY, NULL, NULL);

            current = current->NodeData.Next;
        }

        _Native_TransformMessageProcessor(CM_DESTROY, NULL, NULL);
    }

    return 0;
}

void ComponentBase::_Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
        if (Visible) {
            Render(*(Gdiplus::Graphics*)lParam);
        }
    }
}
