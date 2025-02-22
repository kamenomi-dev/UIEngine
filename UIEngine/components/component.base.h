/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#ifndef __COMPONENT_BASE_H__
#define __COMPONENT_BASE_H__

namespace Engine::Component {

class ComponentBase;

enum class UIENGINE_API ComponentStatusFlags : UINT { None = FLAG_INDEX(0), Visible = FLAG_INDEX(1), Disable = FLAG_INDEX(2) };

struct UIENGINE_API     NodeDataType {
    ComponentBase* Parent;
    ComponentBase* Prev;
    ComponentBase* Next;
    ComponentBase* FirstChild;
    ComponentBase* LastChild;

    NodeDataType() {
        Parent     = nullptr;
        Prev       = nullptr;
        Next       = nullptr;
        FirstChild = nullptr;
        LastChild  = nullptr;
    }
};

struct UIENGINE_API CommonComponentDataType {
    NodeDataType Node{};
    wstring      ComponentID{};

    UINT         StatusFlag{(UINT)ComponentStatusFlags::None};
    Size         ComponentSize{};
    Point        ComponentPosition{};
};

class UIENGINE_API ComponentBase {
  private:
    HWND _nativeWindow{nullptr};

  public:
    friend class Window;

    ComponentBase() { _componentData.ComponentID = L"Component.Base"; }
    ~ComponentBase() {
        ComponentBase::SplitComponentRelation(this);

        auto current = NodeData.FirstChild;
        while (current != nullptr) {
            const auto next = current->NodeData.Next;

            delete current;
            current = next;
        }
    }

  public:
    virtual void Initialize() {};
    void         SetStatusFlag(std::initializer_list<ComponentStatusFlags> flags) { Utils::CombineFlag(_componentData.StatusFlag, flags); }
    void         InsertComponentChild(ComponentBase* child) {
        ComponentBase::SplitComponentRelation(child);

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
    virtual void       _Native_TransformMessageProcessor(UINT, WPARAM, LPARAM);

  public:
    static void SplitComponentRelation(_Inout_ ComponentBase* comp) {
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

    static void DestroyComponentAndChildren(_In_ ComponentBase* comp) {
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
    CommonComponentDataType _componentData{};

  public:
    const CommonComponentDataType& GetBaseData() const { return _componentData; }
    void                           SetBaseData(_In_ const CommonComponentDataType& data) { _componentData = data; }

    const NodeDataType&            GetNodeData() const { return _componentData.Node; }
    void                           SetNodeData(_In_ const NodeDataType& data) { _componentData.Node = data; }

    const UINT&                    GetStatusFlags() const { return _componentData.StatusFlag; }

    bool                           IsVisible() const { return Utils::HasFlag(StatusFlags, ComponentStatusFlags::Visible); };
    void                           SetVisible(_In_ const bool status) {
        if (status) {
            _componentData.StatusFlag |= (UINT)ComponentStatusFlags::Visible;
            return;
        }

        _componentData.StatusFlag &= ~(UINT)ComponentStatusFlags::Visible;
    };

    bool IsDisabled() const { return Utils::HasFlag(StatusFlags, ComponentStatusFlags::Visible); };
    void SetDisabled(_In_ const bool status) {
        if (status) {
            _componentData.StatusFlag |= (UINT)ComponentStatusFlags::Disable;
            return;
        }

        _componentData.StatusFlag &= ~(UINT)ComponentStatusFlags::Disable;
    };

    const Size&  GetComponentSize() const { return _componentData.ComponentSize; }
    void         SetComponentSize(const Size& size) { _componentData.ComponentSize = size; }

    const Point& GetComponentPosition() const { return _componentData.ComponentPosition; }
    void         SetComponentPosition(const Point& pos) { _componentData.ComponentPosition = pos; }

  public:
    COMPONENT_PROPERTY(GetBaseData, SetBaseData) CommonComponentDataType BaseData;
    COMPONENT_PROPERTY(GetNodeData, SetNodeData) NodeDataType            NodeData;
    COMPONENT_PROPERTY_GETTER_ONLY(GetStatusFlags) UINT                  StatusFlags;

    COMPONENT_PROPERTY(IsVisible, SetVisible) bool                       Visible;
    COMPONENT_PROPERTY(IsDisabled, SetDisabled) bool                     Disabled;

    COMPONENT_PROPERTY(GetComponentSize, SetComponentSize) Size          ComponentSize;
    COMPONENT_PROPERTY(GetComponentPosition, SetComponentPosition) Point ComponentPosition;
};
} // namespace Engine::Component

#endif // !__COMPONENT_BASE_H__