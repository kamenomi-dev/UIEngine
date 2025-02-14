/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

namespace Engine::Render {

// SwapBuffer .

class UIENGINE_API SwapBuffer final {
private:
    HWND    _targetWindow{};
    HDC     _targetWindowDC{};
    SIZE    _targetWindowSize{};
    HDC     _swapDC{};
    HBITMAP _swapBitmap{};
    HBITMAP _swapLastBitmap{};

    void _DestroyOldBitmap();

public:
    SwapBuffer(HWND window)
    : _swapDC{CreateCompatibleDC(nullptr)},
      _targetWindow{window},
      _targetWindowDC{GetDC(window)} {
        RefreshSize();
    };
    ~SwapBuffer() {
        _DestroyOldBitmap();

        DeleteDC(_swapDC);
        ReleaseDC(_targetWindow, _targetWindowDC);
    };

    HDC GetRenderableDC() const;

    bool Present();
    bool Present(HDC);
    void RefreshSize();
};
} // namespace Engine::Render

#endif // __ENGINE_RENDER_H__