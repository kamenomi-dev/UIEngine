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
        ComponentVisible  |= false;
        ComponentDisabled |= false;

        ComponentRect = ComponentRect.IsEmptyArea() ? Rect(0, 0, 800, 600) : ComponentRect;
        ComponentLabel =
            ComponentLabel.empty() ? L"UIEngine.Base "s + std::to_wstring(rand() % time(0)) : ComponentLabel;
    }

public:
    void SetChildComponent(CBase* pComponent) {
        CHECK_RESULT_BOOL(pComponent->ComponentClass != L"Window");

        pComponent->_SetParentComponent(this);
        _pChildComponents.push_back(pComponent);
    };
    void SetChildComponents(vector<CBase*> components) {
        _pChildComponents.insert(_pChildComponents.end(), components.begin(), components.end());
    };
    vector<CBase*>& GetChildCompnents() { return _pChildComponents; };
    CBase*          GetParentComponent() { return _pParentComponent; };

    unordered_map<wstring, any>& GetComponentData() { return GetPropertyData(); }

    virtual void    Render(Gdiplus::Graphics&);
    virtual LRESULT _Native_ComponentMessageProcessor(UINT, WPARAM, LPARAM, bool&);
    virtual void    _Native_TransformMessageProcessor(UINT, WPARAM, LPARAM);

private:
    CBase*         _pParentComponent{nullptr};
    vector<CBase*> _pChildComponents{};

    void _SetParentComponent(CBase* pComponent) { _pParentComponent = pComponent; }

    // Property

    bool _isVisible{false};
    bool _isDisabled{false};

    wstring _componentLabel{};
    Rect    _componentRect{};

public:
    COMPONENT_PROPERTY(_Property_IsVisible, _Property_SetVisible) bool ComponentVisible;
    COMPONENT_PROPERTY(_Property_IsDisabled, _Property_SetDisabled) bool ComponentDisabled;

    COMPONENT_PROPERTY_GETTER_ONLY(_Property_GetComponentClass) wstring ComponentClass;
    COMPONENT_PROPERTY(_Property_GetComponentLabel, _Property_SetComponentLabel) wstring ComponentLabel;
    COMPONENT_PROPERTY(_Property_GetComponentRect, _Property_SetComponentRect) Rect ComponentRect;
    COMPONENT_PROPERTY(_Property_GetComponentSize, _Property_SetComponentSize) Size ComponentSize;
    COMPONENT_PROPERTY(_Property_GetComponentPosition, _Property_SetComponentPosition) Point ComponentPosition;


    bool _Property_IsVisible() const { return _isVisible; };
    void _Property_SetVisible(bool newStatus) {
        // Todo! Support dynamic render.
        _isVisible = newStatus;
    };

    bool _Property_IsDisabled() const { return _isDisabled; };
    void _Property_SetDisabled(bool newStatus) {
        // Todo! Support dynamic render.
        _isDisabled = newStatus;
    };

    virtual wstring _Property_GetComponentClass() const { return L"Component.Base"s; }

    wstring _Property_GetComponentLabel() const { return _componentLabel; }
    void    _Property_SetComponentLabel(wstring newLabel) {
        // Todo! Support dynamic render.
        _componentLabel = newLabel;
    }

    Rect& _Property_GetComponentRect() { return _componentRect; }
    Rect  _Property_SetComponentRect(Rect newRect) {
        // Todo! Support dynamic render.
        _componentRect = newRect;

        return newRect;
    }

    Size _Property_GetComponentSize() const { return Size{_componentRect.Width, _componentRect.Height}; }
    Size _Property_SetComponentSize(Size newSize) {
        // Todo! Support dynamic render.
        _componentRect.Width  = newSize.Width;
        _componentRect.Height = newSize.Height;

        return newSize;
    }

    Point _Property_GetComponentPosition() const { return Point{_componentRect.X, _componentRect.Y}; }
    Point _Property_SetComponentPosition(Point newPosition) {
        // Todo! Support dynamic render.
        _componentRect.X = newPosition.X;
        _componentRect.Y = newPosition.Y;

        return newPosition;
    }
};
} // namespace Component
} // namespace Engine

#endif // !__COMPONENT_BASE_H__
