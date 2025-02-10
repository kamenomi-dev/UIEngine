/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __COMPONENT_BASE_H__
#define __COMPONENT_BASE_H__

namespace Engine {
namespace Component {

class UIENGINE_API CBase : public Utils::CProperty {
public:
    CBase(vector<Utils::PropertyPair>);
    ~CBase();

    friend class CWindow;

public:
    inline void            SetChildComponent(CBase*);
    inline void            SetChildComponents(vector<CBase*>);
    inline CBase*          GetChildCompnent();
    inline vector<CBase*>* GetChildCompnents();
    inline CBase*          GetParentComponent();

    unordered_map<wstring, any>& GetComponentData();

    inline virtual wstring GetComponentClass() const;
    void                   SetComponentLabel(wstring);
    wstring                GetComponentLabel() const;

    void         SetComponentSize(Size);
    void         SetComponentPosition(Point);
    inline Rect  GetComponentRect() const;
    inline Size  GetComponentSize() const;
    inline Point GetComponentPosition() const;

    virtual void    Render(Gdiplus::Graphics&);
    virtual LRESULT __Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);
    virtual void    __Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CBase*          _pParentComponent;
    vector<CBase*>* _pChildComponents;

    inline void __SetParentComponent(CBase*);
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_BASE_H__
