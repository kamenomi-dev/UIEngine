#include "pch.h"

#include "engine.h"

using namespace Engine::Utils;

CProperty::CProperty(const vector<PropertyPair>& pairs) {
    for (auto& pair : pairs) {
        SetPropertyByValue(pair.propertyKey, pair.propertyValue);
    }
}

inline void CProperty::SetPropertyByValue(const wstring& key, any value) { _propertyData[key] = value; }

inline void CProperty::SetPropertyByRef(const wstring& key, any& value) { _propertyData[key].swap(value); }

inline void CProperty::SetPropertyIfNotExistByValue(const wstring& key, any value) {
    if (!_propertyData.contains(key)) {
        SetPropertyByValue(key, value);
    }
}

inline void CProperty::SetPropertyIfNotExistByRef(const wstring& key, any& value) {
    if (!_propertyData.contains(key)) {
        SetPropertyByRef(key, value);
    }
}
