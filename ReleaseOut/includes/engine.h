#pragma once
#include "pch.h"

#ifdef UIENGINE_EXPORTS
#define UIENGINE_API __declspec(dllexport)
#else
#define UIENGINE_API __declspec(dllimport)
#endif

#include "./engine.define.h"
#include "./engine.render.h"
#include "./engine.logic.h"
#include "./components/component.base.h"
#include "./components/component.window.h"
#include "./engine.uimanager.h"

#ifndef __ENGINE_H__
#define __ENGINE_H__

#ifdef _DEBUG
#define CHECK_RESULT(ret) assert((ret) == Status::Ok && "Error, need to debug to find the detail.");
#else
#define CHECK_RESULT(ret) ret;
#endif

#ifdef _DEBUG
#define CHECK_RESULT_BOOL(ret) assert(!!(ret) && "Error, need to debug to find the detail.");
#else
#define CHECK_RESULT_BOOL(ret) ret;
#endif

namespace Engine {
extern UINT_PTR  pGdiToken;
extern HINSTANCE hModuleInstance;

UIENGINE_API extern void Initialize(HINSTANCE hInstance);
UIENGINE_API extern void Uninitialize();

// native
void __InitializeEngine();
void __UninitializeEngine();
} // namespace Engine

#endif //  __ENGINE_H__