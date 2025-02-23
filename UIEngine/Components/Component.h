/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __COMPONENT_BASE_H__
#define __COMPONENT_BASE_H__

namespace Engine::Components {

class Component;

struct UIENGINE_API NodeDataType {
    Component* Parent{};
    Component* Prev{};
    Component* Next{};
    Component* FirstChild{};
    Component* LastChild{};
};

struct UIENGINE_API ComponentDataType {
    NodeDataType Node{};
    wstring      ComponentID{};

    CompStatus   StatusFlag{CompStatus::None};
    Size         ComponentSize{};
    Point        ComponentPosition{};
};

class UIENGINE_API Component {
  private:
    HWND _nativeWindow{nullptr};

  public:
    friend class Window;

    Component() { _componentData.ComponentID = L"Component.Base"; }
    ~Component() {
        Component::SplitComponentRelation(this);

        auto current = NodeData.FirstChild;
        while (current != nullptr) {
            const auto next = current->NodeData.Next;

            delete current;
            current = next;
        }
    }

  public:
    virtual void Initialize() {};
    void         SetStatusFlag(CompStatus flags) { _componentData.StatusFlag = flags; }
    void         InsertComponentChild(Component* child) {
        Component::SplitComponentRelation(child);

        child->_componentData.Node.Parent = this;

        if (NodeData.FirstChild == nullptr) {
            _componentData.Node.FirstChild = child;
            _componentData.Node.LastChild  = child;

            return;
        }

        if (NodeData.LastChild != nullptr) {
            _componentData.Node.LastChild->_componentData.Node.Next = child;
            child->_componentData.Node.Prev                         = NodeData.LastChild;

            _componentData.Node.LastChild = child;
        }
    }


  public:
    const wstring_view GetComponentID() const { return BaseData.ComponentID; }

    virtual void       Render(Gdiplus::Graphics&);
    virtual LRESULT    _Native_ComponentMessageProcessor(UINT, WPARAM, LPARAM, bool&);
    virtual void       _Native_TransformMessageProcessor(ComponentMessages, WPARAM, LPARAM);

  public:
    static void SplitComponentRelation(_Inout_ Component* comp) {
        if (comp->NodeData.Parent != nullptr) {
            auto parent = comp->NodeData.Parent;

            if (parent->NodeData.FirstChild == comp) {
                parent->_componentData.Node.FirstChild = comp->NodeData.Next;
            }

            if (parent->NodeData.LastChild == comp) {
                parent->_componentData.Node.LastChild = parent->NodeData.Prev;
            }

            comp->_componentData.Node.Parent = nullptr;
        }

        if (comp->NodeData.Prev != nullptr) comp->_componentData.Node.Prev = comp->NodeData.Next;
        if (comp->NodeData.Next != nullptr) comp->_componentData.Node.Next = comp->NodeData.Prev;

        comp->_componentData.Node.Prev = comp->_componentData.Node.Next = nullptr;
    }

    static void DestroyComponentAndChildren(_In_ Component* comp) {
        delete comp;

        /*
        auto current = comp;
        while (current != nullptr) {
            if (current->NodeData.FirstChild) {
                DestroyComponentAndChildren(comp->NodeData.FirstChild);
            }

            const auto next = current->NodeData.Next;

            delete current;
            current = next;
         }
         */
    }

    /*
     *
     *    Component Data Part.
     *    Base data:
     *        Type -------- Property -------- Access ------ Comment -----------------------------------------------------------
     *        NodeDataType  Node              Getter/Setter The Parent property cannot be changed in the WindowComponent class.
     *        bool          Visible           Getter/Setter    /
     *        bool          Disabled          Getter/Setter    /
     *        Size          ComponentSize     Getter/Setter    /
     *        Point         ComponentPosition Getter/Setter    /
     *
     */

  public:
    ComponentDataType _componentData{};

  public:
    constexpr auto&      GetBaseData() const { return _componentData; }
    constexpr void       SetBaseData(const ComponentDataType& data) { _componentData = data; }

    constexpr auto&      GetNodeData() const { return _componentData.Node; }
    constexpr void       SetNodeData(const NodeDataType& data) { _componentData.Node = data; }

    constexpr CompStatus GetStatusFlags() const { return _componentData.StatusFlag; }

    constexpr bool       IsVisible() const { return Utils::Flags::HasFlag(GetStatusFlags(), CompStatus::Visible); };
    constexpr void       SetVisible(bool status) {
        if (status) {
            _componentData.StatusFlag |= CompStatus::Visible;
            return;
        }

        _componentData.StatusFlag &= ~CompStatus::Visible;
    };

    bool IsDisabled() const { return Utils::Flags::HasFlag(StatusFlags, CompStatus::Visible); };
    void SetDisabled(bool status) {
        if (status) {
            _componentData.StatusFlag |= CompStatus::Disable;
            return;
        }

        _componentData.StatusFlag &= ~CompStatus::Disable;
    };

    auto GetComponentSize() const { return _componentData.ComponentSize; }
    void SetComponentSize(Size size) { _componentData.ComponentSize = size; }

    auto GetComponentPosition() const { return _componentData.ComponentPosition; }
    void SetComponentPosition(Point pos) { _componentData.ComponentPosition = pos; }

  public:
    COMPONENT_PROPERTY(GetBaseData, SetBaseData) ComponentDataType       BaseData;
    COMPONENT_PROPERTY(GetNodeData, SetNodeData) NodeDataType            NodeData;
    COMPONENT_PROPERTY_GETTER_ONLY(GetStatusFlags) CompStatus            StatusFlags;

    COMPONENT_PROPERTY(IsVisible, SetVisible) bool                       Visible;
    COMPONENT_PROPERTY(IsDisabled, SetDisabled) bool                     Disabled;

    COMPONENT_PROPERTY(GetComponentSize, SetComponentSize) Size          ComponentSize;
    COMPONENT_PROPERTY(GetComponentPosition, SetComponentPosition) Point ComponentPosition;
};
} // namespace Engine::Components

#endif // !__COMPONENT_BASE_H__