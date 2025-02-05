#pragma once

#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

namespace Engine {
namespace Render {

// SwapBuffer .

class UIENGINE_API CSwapBuffer : Interface::IUninheritable {
private:
    HWND    _hTargetWnd;
    SIZE    _hTargetSize;
    HDC     _hTargetDC;
    HDC     _hSwapDC;
    HBITMAP _hSwapBitmap;
    HBITMAP _hSwapLastBitmap;

    void __DestroyOldBitmap();

public:
    CSwapBuffer(HWND hWindow);
    ~CSwapBuffer();

    HDC GetRenderableDC() const;

    bool Present();
    bool Present(HDC);
    void RefreshSize();
};
} // namespace Render
} // namespace Engine

#endif // __ENGINE_RENDER_H__