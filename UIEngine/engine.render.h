/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

namespace Engine::Render {

// SwapBuffer .

class UIENGINE_API SwapBuffer final {
private:
    HWND    _hTargetWnd{};
    SIZE    _hTargetSize{};
    HDC     _hTargetDC{};
    HDC     _hSwapDC{};
    HBITMAP _hSwapBitmap{};
    HBITMAP _hSwapLastBitmap{};

    void _DestroyOldBitmap();

public:
    SwapBuffer(HWND hWindow);
    ~SwapBuffer();

    HDC GetRenderableDC() const;

    bool Present();
    bool Present(HDC);
    void RefreshSize();
};
} // namespace Engine::Render

#endif // __ENGINE_RENDER_H__