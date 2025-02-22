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

#ifdef _DEBUG // !!! FIX ME
#define CHECK_RESULT(ret)      assert((ret) == Status::Ok && "Error, need to debug to find the detail.");
#define CHECK_RESULT_BOOL(ret) assert(!!(ret) && "Error, need to debug to find the detail.");
#else
#define CHECK_RESULT(ret)      ret;
#define CHECK_RESULT_BOOL(ret) ret;
#endif

#define FLAG_INDEX(idx) 1u << idx
#define FLAG_MATCH(flags, flag) ((UINT)flags & (UINT)flag) == 1

#define COMPONENT_PROPERTY(Getter, Setter)     __declspec(property(get = Getter, put = Setter))
#define COMPONENT_PROPERTY_GETTER_ONLY(Getter) __declspec(property(get = Getter))
#define COMPONENT_PROPERTY_SETTER_ONLY(Setter) __declspec(property(put = Setter))

namespace Engine {
extern UINT_PTR  uGdiToken;
extern HINSTANCE hModuleInstance;

UIENGINE_API extern void Initialize(HINSTANCE hInstance);
UIENGINE_API extern void Uninitialize();

// native
void InitializeEngineWorker();
void UninitializeEngineWorker();
} // namespace Engine

#include "./engine.enum.h"
#include "./engine.logic.h"
#include "./engine.render.h"
#include "./engine.util.h"

#include "./components/component.base.h"
#include "./components/component.window.h"

#include "./engine.uimanager.h"

#endif //  __ENGINE_H__