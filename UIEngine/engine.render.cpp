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

using namespace Engine::Render;

/*
 *  Class.
 *  SwapBuffer 
 */

HDC SwapBuffer::GetRenderableDC() const { return _swapDC; }

bool SwapBuffer::Present() {
    return BitBlt(_targetWindowDC, 0, 0, _targetWindowSize.cx, _targetWindowSize.cy, _swapDC, 0, 0, SRCCOPY);
}

bool SwapBuffer::Present(HDC hTargetDC) {
    return BitBlt(hTargetDC, 0, 0, _targetWindowSize.cx, _targetWindowSize.cy, _swapDC, 0, 0, SRCCOPY);
}

void SwapBuffer::RefreshSize() {
    RECT rcWnd{};
    GetWindowRect(_targetWindow, &rcWnd);

    _DestroyOldBitmap();
    _targetWindowSize.cx  = rcWnd.right - rcWnd.left - 1;
    _targetWindowSize.cy  = rcWnd.bottom - rcWnd.top - 1;
    _swapBitmap     = CreateCompatibleBitmap(_targetWindowDC, _targetWindowSize.cx, _targetWindowSize.cy);
    _swapLastBitmap = SelectBitmap(_swapDC, _swapBitmap);
}

void SwapBuffer::_DestroyOldBitmap() {
    if (_swapLastBitmap) {
        SelectBitmap(_swapDC, _swapLastBitmap);
    }
    DeleteBitmap(_swapBitmap);

    _swapBitmap     = nullptr;
    _swapLastBitmap = nullptr;
}
