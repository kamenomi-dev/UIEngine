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