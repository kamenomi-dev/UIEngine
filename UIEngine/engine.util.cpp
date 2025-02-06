#include "pch.h"

#include "engine.h"

using namespace Engine::Utils;

CProperty::CProperty(vector<PropertyPair> pairs) : _propertyData(new unordered_map<wstring, any>) {
    for (auto& pair : pairs) {
        SetPropertyByValue(pair.propertyKey, pair.propertyValue);
    }
}

CProperty::~CProperty() {
    delete _propertyData;
    _propertyData = nullptr;
}

inline void CProperty::SetPropertyByValue(wstring key, any value) { (*_propertyData)[key] = value; }

inline void CProperty::SetPropertyByRef(wstring key, any& value) { (*_propertyData)[key].swap(value); }

inline void CProperty::SetPropertyIfNotExistByValue(wstring key, any value) {
    if ((*_propertyData).find(key) == (*_propertyData).end()) {
        SetPropertyByValue(key, value);
    }
}

inline void CProperty::SetPropertyIfNotExistByRef(wstring key, any& value) {
    if ((*_propertyData).find(key) == (*_propertyData).end()) {
        SetPropertyByRef(key, value);
    }
}

any& CProperty::GetProperty(wstring key) {
    return (*_propertyData)[key];
}

const any& CProperty::GetProperty(wstring key) const {
    return (*_propertyData)[key];
}

unordered_map<wstring, any>* CProperty::GetPropertyData() const { return _propertyData; }
