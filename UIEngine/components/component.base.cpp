#include "pch.h"

#include "../engine.h"

using namespace Engine::Component;

// Component Initialization

CBase::CBase(vector<Utils::PropertyPair> pairs)
: CProperty(pairs),
  _pParentComponent(nullptr),
  _pChildComponents(new std::vector<CBase*>) {
    SetPropertyIfNotExistByValue(L"visible", false);
    SetPropertyIfNotExistByValue(L"disabled", false);
    SetPropertyIfNotExistByValue(L"componentRect", make_any<Rect>(0, 0, 800, 600));
    SetPropertyIfNotExistByValue(L"componentLabel", L"UIEngine.Base");
}

CBase::~CBase() {}

// Component Identity Settger/Getter

wstring CBase::GetComponentClass() const { return L"Base"; }

void CBase::SetComponentLabel(wstring newLabel) { SetPropertyIfNotExistByValue(L"componentLabel", newLabel); }

wstring CBase::GetComponentLabel() const { return GetPropertyTyped<wstring>(L"componentLabel"); }

unordered_map<wstring, any>& CBase::GetComponentData() { return GetPropertyData(); }

// Component Rectangle Setter/Getter

void CBase::SetComponentSize(Size newSize) {
    auto& componentRect  = GetPropertyTyped<Rect>(L"componentRect");
    componentRect.Width  = newSize.Width;
    componentRect.Height = newSize.Height;
}

void CBase::SetComponentPosition(Point newPosition) {
    auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

    componentRect.X = newPosition.X;
    componentRect.Y = newPosition.Y;
}

Size CBase::GetComponentSize() const {
    const auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");
    Size        returnSize{};

    componentRect.GetSize(&returnSize);

    return returnSize;
}

Point CBase::GetComponentPosition() const {
    const auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");
    Point      returnPosition{};

    componentRect.GetLocation(&returnPosition);

    return returnPosition;
}

// Component Family

inline void CBase::SetChildComponent(CBase* pComponent) {
    CHECK_RESULT_BOOL(pComponent->GetComponentClass() != L"Window");

    pComponent->__SetParentComponent(this);
    _pChildComponents->push_back(pComponent);
}

inline void CBase::SetChildComponents(vector<CBase*> components) {
    _pChildComponents->insert(_pChildComponents->end(), components.begin(), components.end());
}

inline CBase* CBase::GetChildCompnent() { return nullptr; }

inline vector<CBase*>* CBase::GetChildCompnents() { return _pChildComponents; }

inline CBase* CBase::GetParentComponent() { return _pParentComponent; }

inline void CBase::__SetParentComponent(CBase* pComponent) { _pParentComponent = pComponent; }

// Component Message

void Engine::Component::CBase::Render(Gdiplus::Graphics&) {}

LRESULT CBase::__Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn) {
    bIsReturn = false;
    return NULL;
}

void CBase::__Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {}
