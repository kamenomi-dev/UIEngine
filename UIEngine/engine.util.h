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

    void SetPropertyByValue(const wstring&, any);
    void SetPropertyByRef(const wstring&, any&);

    void SetPropertyIfNotExistByValue(const wstring&, any);
    void SetPropertyIfNotExistByRef(const wstring&, any&);

    any& GetProperty(const wstring& key) {
        const auto it = _propertyData->find(key);
        if (it == _propertyData->end()) {
            DebugBreak();
            abort();
        }
        return it->second;
    }
    const any& GetProperty(const wstring& key) const {
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
    auto& GetPropertyTyped(const wstring& key) {
        return any_cast<T&>(GetProperty(key));
    }
    template <class T>
    auto& GetPropertyTyped(const wstring& key) const {
        return any_cast<const T&>(GetProperty(key));
    }

private:
    unique_ptr<unordered_map<wstring, any>> _propertyData;
};

} // namespace Engine::Utils

#endif // !__ENGINE_UTIL_H__