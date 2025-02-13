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

UINT_PTR  Engine::uGdiToken{};
HINSTANCE Engine::hModuleInstance{};

UIENGINE_API void Engine::Initialize(HINSTANCE hInstance) {
    hModuleInstance = hInstance;

    if (!uGdiToken) {
        Engine::InitializeEngineWorker();
    }

    return;
}

UIENGINE_API void Engine::Uninitialize() {
    UninitializeEngineWorker();
    return;
}

void Engine::InitializeEngineWorker() {
    const Gdiplus::GdiplusStartupInput startupInput{};
    CHECK_RESULT(Gdiplus::GdiplusStartup(&uGdiToken, &startupInput, nullptr));
}

void Engine::UninitializeEngineWorker() {
    if (uGdiToken) {
        Gdiplus::GdiplusShutdown(uGdiToken);
        uGdiToken = 0u;
    }
}