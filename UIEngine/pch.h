#ifndef PCH_H
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

#include <any>
#include <assert.h>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Gdiplus::Point;
using Gdiplus::Rect;
using Gdiplus::Size;
using Gdiplus::Status;

using std::any;
using std::any_cast;
using std::bad_any_cast;
using std::exception;
using std::function;
using std::make_any;
using std::nullptr_t;
using std::string;
using std::unordered_map;
using std::vector;
using std::wstring;

using namespace std::literals;

#endif // PCH_H
