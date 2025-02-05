#include "pch.h"

#include "engine.h"

UINT_PTR  Engine::pGdiToken{NULL};
HINSTANCE Engine::hModuleInstance{NULL};

void __stdcall GdiplusDebugEvent(Gdiplus::DebugEventLevel level, CHAR* message);

UIENGINE_API void Engine::Initialize(HINSTANCE hInstance) {
    hModuleInstance = hInstance;

    if (pGdiToken == NULL) {
        Engine::__InitializeEngine();
    }

    return;
}

UIENGINE_API void Engine::Uninitialize() {
    __UninitializeEngine();
    return;
}

void Engine::__InitializeEngine() {
    Gdiplus::GdiplusStartupInput startupInput{};
    startupInput.GdiplusVersion = 1;

#ifdef _DEBUG
    startupInput.DebugEventCallback = &GdiplusDebugEvent;
#endif // DEBUG

    CHECK_RESULT(Gdiplus::GdiplusStartup(&pGdiToken, &startupInput, nullptr));
}

void Engine::__UninitializeEngine() {
    if (pGdiToken != NULL) {
        Gdiplus::GdiplusShutdown(pGdiToken);
        pGdiToken = NULL;
    }
}

void CALLBACK GdiplusDebugEvent(Gdiplus::DebugEventLevel level, CHAR* message) { OutputDebugStringA(message); }