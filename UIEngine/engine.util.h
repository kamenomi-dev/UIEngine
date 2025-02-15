/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UTIL_H__

namespace Engine::Utils {

struct PropertyPair {
    wstring propertyKey;
    any     propertyValue;
};

class UIENGINE_API CProperty {
public:
    CProperty(const vector<PropertyPair>&);

    void SetPropertyByValue(const wstring_view& key, any);
    void SetPropertyByRef(const wstring_view& key, any&);

    void SetPropertyIfNotExistByValue(const wstring_view& key, any);
    void SetPropertyIfNotExistByRef(const wstring_view& key, any&);

    any& GetProperty(const wstring_view& key) {
        const auto it = _propertyData->find(key);
        if (it == _propertyData->end()) {
            DebugBreak();
            abort();
        }
        return it->second;
    }
    const any& GetProperty(const wstring_view& key) const {
        const auto it = _propertyData->find(key);
        if (it == _propertyData->end()) {
            DebugBreak();
            abort();
        }
        return it->second;
    }

    auto& GetPropertyData() { return *_propertyData; }
    auto& GetPropertyData() const { return *_propertyData; }

    template <class T>
    auto& GetPropertyTyped(const wstring_view& key) {
        return any_cast<T&>(GetProperty(key));
    }
    template <class T>
    auto& GetPropertyTyped(const wstring_view& key) const {
        return any_cast<const T&>(GetProperty(key));
    }

private:
    unique_ptr<unordered_map<wstring_view, any>> _propertyData{new unordered_map<wstring_view, any>};
};

} // namespace Engine::Utils

#endif // !__ENGINE_UTIL_H__