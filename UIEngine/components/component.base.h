/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __COMPONENT_BASE_H__
#define __COMPONENT_BASE_H__

namespace Engine ::Component {

struct BasePropertyDataType {
    bool           Visible;
    bool           Disabled;
    Rect           ComponentRect;
    wstring        ComponentLabel;
    CBase*         ComponentParent;
    vector<CBase*> ComponentChildren;
};

class UIENGINE_API CBase {
public:
    friend class CWindow;

    CBase() {
        Visible  = false;
        Disabled = false;

        if (ComponentRect.IsEmptyArea()) {
            ComponentRect = Rect(0, 0, 800, 600);
        }

        if (ComponentLabel.empty()) {
            ComponentLabel = L"Component.Base";
        }
    }

public:
    virtual void Initialize(){};

    virtual void    Render(Gdiplus::Graphics&);
    virtual LRESULT _Native_ComponentMessageProcessor(UINT, WPARAM, LPARAM, bool&);
    virtual void    _Native_TransformMessageProcessor(UINT, WPARAM, LPARAM);

    /*
     *
     *   Component Property Part.
     *
     */

public:
    BasePropertyDataType _basePropertyData{};

public:
    BasePropertyDataType&       _Property_GetBasePropertyData() { return _basePropertyData; }
    const BasePropertyDataType& _Property_GetBasePropertyData() const { return _basePropertyData; }

    bool _Property_IsVisible() const { return BaseProperty.Visible; };
    void _Property_SetVisible(bool newStatus) {
        // Todo! Support dynamic render.
        BaseProperty.Visible = newStatus;
    };

    bool _Property_IsDisabled() const { return BaseProperty.Disabled; };
    void _Property_SetDisabled(bool newStatus) {
        // Todo! Support dynamic render.
        BaseProperty.Disabled = newStatus;
    };

    virtual wstring _Property_GetComponentClass() const { return L"Component.Base"s; }

    CBase*       _Property_GetComponentParent() { return BaseProperty.ComponentParent; }
    const CBase* _Property_GetComponentParent() const { return BaseProperty.ComponentParent; }
    void         _Property_SetComponentParent(CBase* parent) {
        // Todo! Support dynamic render.

        BaseProperty.ComponentParent = parent;
        parent->ComponentChildren    = parent;
    }

    vector<CBase*>&       _Property_GetComponentChildren() { return BaseProperty.ComponentChildren; };
    const vector<CBase*>& _Property_GetComponentChildren() const { return BaseProperty.ComponentChildren; };
    void                  _Property_SetComponentChildren(CBase* child) {
        child->ComponentParent = this;
        BaseProperty.ComponentChildren.push_back(child);
    }
    void _Property_SetComponentChildren(vector<CBase*> children) {
        for (auto& child : children) {
            child->ComponentParent = this;
        }

        BaseProperty.ComponentChildren.insert(BaseProperty.ComponentChildren.end(), children.begin(), children.end());
    }

    wstring _Property_GetComponentLabel() const { return BaseProperty.ComponentLabel; }
    void    _Property_SetComponentLabel(wstring newLabel) {
        // Todo! Support dynamic render.
        BaseProperty.ComponentLabel = newLabel;
    }

    Rect& _Property_GetComponentRect() { return BaseProperty.ComponentRect; }
    const Rect& _Property_GetComponentRect() const { return BaseProperty.ComponentRect; }
    Rect& _Property_SetComponentRect(Rect newRect) {
        // Todo! Support dynamic render.
        BaseProperty.ComponentRect = newRect;

        return BaseProperty.ComponentRect;
    }

    Size _Property_GetComponentSize() const {
        Size returnSize{};

        ComponentRect.GetSize(&returnSize);

        return returnSize;
    }
    Size _Property_SetComponentSize(Size newSize) {
        // Todo! Support dynamic render.
        BaseProperty.ComponentRect.Width  = newSize.Width;
        BaseProperty.ComponentRect.Height = newSize.Height;

        return newSize;
    }

    Point _Property_GetComponentPosition() const {
        Point returnPosition{};

        ComponentRect.GetLocation(&returnPosition);

        return returnPosition;
    }
    Point _Property_SetComponentPosition(Point newPosition) {
        // Todo! Support dynamic render.
        _basePropertyData.ComponentRect.X = newPosition.X;
        _basePropertyData.ComponentRect.Y = newPosition.Y;

        return newPosition;
    }

    COMPONENT_PROPERTY(_Property_GetBasePropertyData, _Property_SetBasePropertyData)
    BasePropertyDataType BaseProperty;

    COMPONENT_PROPERTY(_Property_IsVisible, _Property_SetVisible) bool Visible;
    COMPONENT_PROPERTY(_Property_IsDisabled, _Property_SetDisabled) bool Disabled;

    COMPONENT_PROPERTY(_Property_GetComponentParent, _Property_SetComponentParent) CBase* ComponentParent;
    COMPONENT_PROPERTY(_Property_GetComponentChildren, _Property_SetComponentChildren) vector<CBase*> ComponentChildren;

    COMPONENT_PROPERTY(_Property_GetComponentLabel, _Property_SetComponentLabel) wstring ComponentLabel;
    COMPONENT_PROPERTY(_Property_GetComponentRect, _Property_SetComponentRect) Rect ComponentRect;
    COMPONENT_PROPERTY(_Property_GetComponentSize, _Property_SetComponentSize) Size ComponentSize;
    COMPONENT_PROPERTY(_Property_GetComponentPosition, _Property_SetComponentPosition) Point ComponentPosition;
    COMPONENT_PROPERTY_GETTER_ONLY(_Property_GetComponentClass) wstring ComponentClass;
};
} // namespace Engine::Component

#endif // !__COMPONENT_BASE_H__