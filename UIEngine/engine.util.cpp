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

using namespace Engine::Utils;

CProperty::CProperty(const vector<PropertyPair>& pairs) {
    _propertyData = make_unique<unordered_map<wstring, any>>();

    for (auto& pair : pairs) {
        SetPropertyByValue(pair.propertyKey, pair.propertyValue);
    }
}

inline void CProperty::SetPropertyByValue(const wstring& key, any value) { (*_propertyData)[key] = value; }

inline void CProperty::SetPropertyByRef(const wstring& key, any& value) { (*_propertyData)[key].swap(value); }

inline void CProperty::SetPropertyIfNotExistByValue(const wstring& key, any value) {
    if (!_propertyData->contains(key)) {
        SetPropertyByValue(key, value);
    }
}

inline void CProperty::SetPropertyIfNotExistByRef(const wstring& key, any& value) {
    if (!_propertyData->contains(key)) {
        SetPropertyByRef(key, value);
    }
}
