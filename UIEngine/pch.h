﻿#ifndef PCH_H
#define PCH_H

// Must support.
#ifndef UNICODE
#define UNICODE
#endif // UNICODE

// For dependency.

#include <Windows.h>
#include <Windowsx.h>
#include <dwmapi.h>
#include <gdiplus.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdiplus.lib")

#include <assert.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using Gdiplus::Point;
using Gdiplus::Rect;
using Gdiplus::Size;
using Gdiplus::Status;

using std::function;
using std::string;
using std::unordered_map;
using std::vector;
using std::wstring;

#endif // PCH_H
