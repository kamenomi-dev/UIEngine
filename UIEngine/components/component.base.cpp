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
    SetPropertyIfNotExistByValue(L"componentRect", Rect({0, 0, 800, 600}));
    SetPropertyIfNotExistByValue(L"componentLabel", L"UIEngine.Base");
}

CBase::~CBase() {}

// Component Identity Settger/Getter

wstring CBase::GetComponentClass() const { return L"Base"; }

void CBase::SetComponentLabel(wstring newLabel) { SetPropertyIfNotExistByValue(L"componentLabel", newLabel); }

wstring CBase::GetComponentLabel() const { return CProperty_GetProperty(L"componentLabel", wstring); }

unordered_map<wstring, any>* CBase::GetComponentData() const { return GetPropertyData(); }

// Component Rectangle Setter/Getter

void CBase::SetComponentSize(Size newSize) {
    auto rect = CProperty_GetProperty(L"componentRect", Rect);
    rect.Width  = newSize.Width;
    rect.Height = newSize.Height;

    SetPropertyByValue(L"componentRect", rect);
}

void CBase::SetComponentPosition(Point newPosition) {
    auto rect = CProperty_GetProperty(L"componentRect", Rect);
    rect.X    = newPosition.X;
    rect.Y    = newPosition.Y;

    SetPropertyByValue(L"componentRect", rect);
}

Size CBase::GetComponentSize() const {
    const auto rect = CProperty_GetProperty(L"componentRect", Rect);
    Size       retSize{};

    rect.GetSize(&retSize);

    return retSize;
}

Point CBase::GetComponentPosition() const {
    const auto rect = CProperty_GetProperty(L"componentRect", Rect);
    Point      retPosition{};

    rect.GetLocation(&retPosition);

    return retPosition;
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
