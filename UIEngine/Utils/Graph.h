/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UTILS_GRAPH_H__
#define __ENGINE_UTILS_GRAPH_H__

namespace Engine::Utils::Graph {

// @brief Covert Microsoft RECT to Gdiplus Rect.
// @param const RECT& ¡ª source
// @return Gdiplus::Rect ¡ª destination
inline [[nodiscard]] Rect RectToGpRect(const RECT& source) { return Rect(source.left, source.top, source.right - source.left, source.bottom - source.top); }

// Common swapbuffer class.
class UIENGINE_API SwapBuffer final {
  public:
    SwapBuffer(HWND window) : _swapDC{CreateCompatibleDC(nullptr)}, _targetWindow{window}, _targetWindowDC{GetDC(window)} { RefreshSize(); };
    ~SwapBuffer() {
        _DestroyOldBitmap();

        DeleteDC(_swapDC);
        ReleaseDC(_targetWindow, _targetWindowDC);
    };

    HDC  GetDrawContext() const;

    bool Present();
    bool Present(HDC);
    void RefreshSize();

  private:
    HWND    _targetWindow{};
    HDC     _targetWindowDC{};
    SIZE    _targetWindowSize{};
    HDC     _swapDC{};
    HBITMAP _swapBitmap{};
    HBITMAP _swapLastBitmap{};

    void    _DestroyOldBitmap();
};

} // namespace Engine::Utils::Graph

#endif // !__ENGINE_UTILS_GRAPH_H__