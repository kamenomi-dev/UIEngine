#include "pch.h"

#include "engine.h"

using namespace Engine::Render;

/*
 *  Class.
 *  SwapBuffer 
 */

CSwapBuffer::CSwapBuffer(HWND hWindow)
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

CSwapBuffer::~CSwapBuffer() {
    __DestroyOldBitmap();
    DeleteDC(_hSwapDC);
    ReleaseDC(_hTargetWnd, _hTargetDC);
}

HDC CSwapBuffer::GetRenderableDC() const { return _hSwapDC; }

bool CSwapBuffer::Present() {
    return BitBlt(_hTargetDC, 0, 0, _hTargetSize.cx, _hTargetSize.cy, _hSwapDC, 0, 0, SRCCOPY);
}

bool CSwapBuffer::Present(HDC hTargetDC) {
    return BitBlt(hTargetDC, 0, 0, _hTargetSize.cx, _hTargetSize.cy, _hSwapDC, 0, 0, SRCCOPY);
}

void CSwapBuffer::RefreshSize() {
    RECT rcWnd{};
    GetWindowRect(_hTargetWnd, &rcWnd);

    __DestroyOldBitmap();
    _hTargetSize.cx  = rcWnd.right - rcWnd.left - 1;
    _hTargetSize.cy  = rcWnd.bottom - rcWnd.top - 1;
    _hSwapBitmap     = CreateCompatibleBitmap(_hTargetDC, _hTargetSize.cx, _hTargetSize.cy);
    _hSwapLastBitmap = SelectBitmap(_hSwapDC, _hSwapBitmap);
}

void CSwapBuffer::__DestroyOldBitmap() {
    if (_hSwapLastBitmap != NULL) {
        SelectBitmap(_hSwapDC, _hSwapLastBitmap);
    }
    DeleteBitmap(_hSwapBitmap);

    _hSwapBitmap     = NULL;
    _hSwapLastBitmap = NULL;
}
