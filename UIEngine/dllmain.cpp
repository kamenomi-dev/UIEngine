// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "engine.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulCallSource, LPVOID lpReserved) {
  DisableThreadLibraryCalls(hModule);

  switch (ulCallSource) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}
