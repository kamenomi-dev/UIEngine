#pragma once

#ifndef __ENGINE_UTIL_H__

#define CProperty_GetProperty(key, type) any_cast<type>(GetProperty(key))

#define CProperty_GetProperty_WithInstance(ptr, key, type) any_cast<type>(ptr->GetProperty(key))

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
    CProperty(vector<PropertyPair>);
    ~CProperty();

    inline void SetPropertyByValue(wstring, any);
    inline void SetPropertyByRef(wstring, any&);

    inline void SetPropertyIfNotExistByValue(wstring, any);
    inline void SetPropertyIfNotExistByRef(wstring, any&);

    inline any&       GetProperty(wstring);
    inline const any& GetProperty(wstring) const;

    unordered_map<wstring, any>* GetPropertyData() const;

private:
    unordered_map<wstring, any>* _propertyData;
};

} // namespace Utils
} // namespace Engine

#endif // !__ENGINE_UTIL_H__
