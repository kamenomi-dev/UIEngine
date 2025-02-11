/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#ifndef __ENGINE_UTIL_H__

namespace Engine {

namespace Utils {

namespace Defines {

typedef struct __structPropertyPair {
    wstring propertyKey;
    any     propertyValue;
} PropertyPair;

} // namespace Defines

using namespace Defines;

class UIENGINE_API CProperty {
public:
    CProperty(const vector<PropertyPair>&);

    inline void SetPropertyByValue(const wstring&, any);
    inline void SetPropertyByRef(const wstring&, any&);

    inline void SetPropertyIfNotExistByValue(const wstring&, any);
    inline void SetPropertyIfNotExistByRef(const wstring&, any&);

    inline any& GetProperty(const wstring& key) {
        const auto it = _propertyData->find(key);
        if (it == _propertyData->end()) {
            DebugBreak();
            abort();
        }
        return it->second;
    }
    inline const any& GetProperty(const wstring& key) const {
        const auto it = _propertyData->find(key);
        if (it == _propertyData->end()) {
            DebugBreak();
            abort();
        }
        return it->second;
    }

    inline auto& GetPropertyData() { return *_propertyData; }
    inline auto& GetPropertyData() const { return *_propertyData; }

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

} // namespace Utils
} // namespace Engine

#endif // !__ENGINE_UTIL_H__