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

SwapBuffer::SwapBuffer(HWND hWindow)
: _hTargetWnd(hWindow),
  _hTargetSize({0, 0}),
  _hTargetDC(NULL),
  _hSwapDC(NULL),
  _hSwapBitmap(NULL),
  _hSwapLastBitmap(NULL) {
    _hTargetDC = GetDC(hWindow);
    _hSwapDC   = CreateCompatibleDC(NULL);

    RefreshSize();
};

SwapBuffer::~SwapBuffer() {
    _DestroyOldBitmap();
    DeleteDC(_hSwapDC);
    ReleaseDC(_hTargetWnd, _hTargetDC);
}

HDC SwapBuffer::GetRenderableDC() const { return _hSwapDC; }

bool SwapBuffer::Present() {
    return BitBlt(_hTargetDC, 0, 0, _hTargetSize.cx, _hTargetSize.cy, _hSwapDC, 0, 0, SRCCOPY);
}

bool SwapBuffer::Present(HDC hTargetDC) {
    return BitBlt(hTargetDC, 0, 0, _hTargetSize.cx, _hTargetSize.cy, _hSwapDC, 0, 0, SRCCOPY);
}

void SwapBuffer::RefreshSize() {
    RECT rcWnd{};
    GetWindowRect(_hTargetWnd, &rcWnd);

    _DestroyOldBitmap();
    _hTargetSize.cx  = rcWnd.right - rcWnd.left - 1;
    _hTargetSize.cy  = rcWnd.bottom - rcWnd.top - 1;
    _hSwapBitmap     = CreateCompatibleBitmap(_hTargetDC, _hTargetSize.cx, _hTargetSize.cy);
    _hSwapLastBitmap = SelectBitmap(_hSwapDC, _hSwapBitmap);
}

void SwapBuffer::_DestroyOldBitmap() {
    if (_hSwapLastBitmap != NULL) {
        SelectBitmap(_hSwapDC, _hSwapLastBitmap);
    }
    DeleteBitmap(_hSwapBitmap);

    _hSwapBitmap     = NULL;
    _hSwapLastBitmap = NULL;
}
