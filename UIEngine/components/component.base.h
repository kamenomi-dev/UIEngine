/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __COMPONENT_BASE_H__
#define __COMPONENT_BASE_H__

namespace Engine {
namespace Component {

class UIENGINE_API CBase : public Utils::CProperty {
public:
    friend class CWindow;

    CBase(vector<Utils::PropertyPair> pairs) : CProperty(pairs) {
        SetPropertyIfNotExistByValue(L"visible", false);
        SetPropertyIfNotExistByValue(L"disabled", false);
        SetPropertyIfNotExistByValue(L"componentRect", make_any<Rect>(0, 0, 800, 600));
        SetPropertyIfNotExistByValue(L"componentLabel", L"UIEngine.Base "s + std::to_wstring(rand() % time(0)));
    }

public:
    inline void SetChildComponent(CBase* pComponent) {
        CHECK_RESULT_BOOL(pComponent->GetComponentClass() != L"Window");

        pComponent->__SetParentComponent(this);
        _pChildComponents.push_back(pComponent);
    };
    inline void SetChildComponents(vector<CBase*> components) {
        _pChildComponents.insert(_pChildComponents.end(), components.begin(), components.end());
    };
    inline vector<CBase*>& GetChildCompnents() { return _pChildComponents; };
    inline CBase*          GetParentComponent() { return _pParentComponent; };

    inline unordered_map<wstring, any>& GetComponentData() { return GetPropertyData(); }

    inline virtual wstring GetComponentClass() const { return L"Base"s; };
    inline wstring         GetComponentLabel() const { return GetPropertyTyped<wstring>(L"componentLabel"); };
    void                   SetComponentLabel(wstring newLabel) { SetPropertyByValue(L"componentLabel", newLabel); }

    void SetComponentSize(Size newSize) {
        auto& componentRect  = GetPropertyTyped<Rect>(L"componentRect");
        componentRect.Width  = newSize.Width;
        componentRect.Height = newSize.Height;
    }
    void SetComponentPosition(Point newPosition) {
        auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

        componentRect.X = newPosition.X;
        componentRect.Y = newPosition.Y;
    }
    inline Rect GetComponentRect() const { return GetPropertyTyped<Rect>(L"componentRect"); };
    inline Size GetComponentSize() const {
        const auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");
        Size        returnSize{};

        componentRect.GetSize(&returnSize);

        return returnSize;
    }
    inline Point GetComponentPosition() const {
        const auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");
        Point       returnPosition{};

        componentRect.GetLocation(&returnPosition);

        return returnPosition;
    };

    virtual void    Render(Gdiplus::Graphics&);
    virtual LRESULT _Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn);
    virtual void    _Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CBase*         _pParentComponent{nullptr};
    vector<CBase*> _pChildComponents{};

    inline void __SetParentComponent(CBase* pComponent) { _pParentComponent = pComponent; }
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_BASE_H__
