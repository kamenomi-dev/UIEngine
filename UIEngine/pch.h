/* SPDX-License-Identifier: GPL-3.0-or-later */
#ifndef PCH_H
#define PCH_H

#pragma message("Compiling " __DATE__)
#pragma message("Last modified on " __TIMESTAMP__)

// Skip Something.

#ifndef ___NO_SKIP_4251
#pragma warning(disable : 4251)
#pragma message("Oh! The library has ignored C4251!"                                                                   \
                " You should add '___NO_SKIP_4251' marco"                                                              \
                " at the topest level of file if you get in trouble.")
#endif

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

#include <algorithm>
#include <any>
#include <assert.h>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

using Gdiplus::Point;
using Gdiplus::Rect;
using Gdiplus::Size;
using Gdiplus::Status;

using std::make_unique;
using std::unique_ptr;

using std::any;
using std::any_cast;
using std::bad_any_cast;
using std::make_any;

using std::exception;
using std::function;
using std::runtime_error;

using std::string;
using std::wstring;

using std::reverse;
using std::unordered_map;
using std::vector;

using namespace std::literals;

#endif // PCH_H
