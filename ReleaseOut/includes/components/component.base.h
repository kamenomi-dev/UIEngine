#pragma once
#ifndef __COMPONENT_BASE_H__
#define __COMPONENT_BASE_H__

namespace Engine {
namespace Component {

namespace Defines {
typedef struct __structPropertyPair {
    wstring propertyKey;
    void*   propertyValuePtr;
} PropertyPair;
} // namespace Defines

using namespace Defines;

class UIENGINE_API CBase {
public:
    CBase(vector<PropertyPair>);
    ~CBase();

    friend class CWindow;

public:
    inline void            SetChildComponent(CBase*);
    inline void            SetChildComponents(vector<CBase*>);
    inline CBase*          GetChildCompnent();
    inline vector<CBase*>* GetChildCompnents();
    inline CBase*          GetParentComponent();

    void                           SetProperty(wstring, void*);
    void*                          GetProperty(wstring) const;
    virtual wstring                GetComponentClass() const;
    void                           SetComponentLabel(wstring);
    wstring                        GetComponentLabel() const;
    unordered_map<wstring, void*>* GetComponentData() const;

    void  SetComponentSize(Size);
    void  SetComponentPosition(Point);
    Size  GetComponentSize() const;
    Point GetComponentPosition() const;

    virtual LRESULT
    __Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);
    virtual void __Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    unordered_map<wstring, void*>* _componentData;

    CBase*               _pParentComponent;
    std::vector<CBase*>* _pChildComponents;

    inline void __SetParentComponent(CBase*);
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_BASE_H__
