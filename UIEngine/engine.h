/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include "pch.h"

#ifdef UIENGINE_EXPORTS
#define UIENGINE_API __declspec(dllexport)
#else
#define UIENGINE_API __declspec(dllimport)
#endif

#ifndef __ENGINE_H__
#define __ENGINE_H__

#ifdef _DEBUG
#define CHECK_RESULT(ret) assert((ret) == Status::Ok && "Error, need to debug to find the detail.");
#define CHECK_RESULT_BOOL(ret) assert(!!(ret) && "Error, need to debug to find the detail.");
#else
#define CHECK_RESULT(ret) ret;
#define CHECK_RESULT_BOOL(ret) ret;
#endif

namespace Engine {
extern UINT_PTR  uGdiToken;
extern HINSTANCE hModuleInstance;

UIENGINE_API extern void Initialize(HINSTANCE hInstance);
UIENGINE_API extern void Uninitialize();

// native
void InitializeEngineWorker();
void UninitializeEngineWorker();
} // namespace Engine

#include "./engine.util.h"
#include "./engine.enum.h"
#include "./engine.render.h"
#include "./engine.logic.h"
#include "./components/component.base.h"
#include "./components/component.window.h"
#include "./engine.uimanager.h"

#endif //  __ENGINE_H__