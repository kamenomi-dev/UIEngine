#include "pch.h"

#include "../engine.h"

using namespace Engine::Component;

// Component Initialization

CBase::CBase(vector<PropertyPair> pairs)
: _pParentComponent(nullptr),
  _pChildComponents(new std::vector<CBase*>),
  _componentData(new unordered_map<wstring, void*>) {

    for (size_t idx = 0; idx < pairs.size(); idx++) {
        const auto& pair = pairs[idx];
        _componentData->emplace(pair.propertyKey, pair.propertyValuePtr);
    }

    _componentData->emplace(L"visible", new bool(false));
    _componentData->emplace(L"disabled", new bool(false));
    _componentData->emplace(L"componentRect", new Rect({0, 0, 800, 600}));
    _componentData->emplace(L"componentLabel", new wstring(L"UIEngine.Base"));
}

CBase::~CBase() {
    if (_componentData != nullptr) {

        for (auto& pair : *_componentData) {
            delete pair.second;
        }
        _componentData->clear();

        delete _componentData;
        _componentData = nullptr;
    }
}

// Component Identity Settger/Getter

void CBase::SetProperty(wstring propertyKey, void* newData) {
    auto pLastData = GetProperty(propertyKey);

    if (pLastData != nullptr) {
        delete pLastData;
        pLastData = NULL;
    }

    try {
        (*_componentData)[propertyKey] = newData;
    } catch (const std::exception&) {
        assert(false && "Stop! You are using a invalied property key.");
    }
}

void* CBase::GetProperty(wstring propertyKey) const {
    try {
        return _componentData->at(propertyKey);
    } catch (const std::exception&) {
        assert(false && "Stop! You are using a invalied property key.");
        return nullptr;
    }
}

wstring CBase::GetComponentClass() const { return L"Base"; }

void CBase::SetComponentLabel(wstring newLabel) { SetProperty(L"componentLabel", new wstring(newLabel)); }

wstring CBase::GetComponentLabel() const { return *(wstring*)GetProperty(L"componentLabel"); }

unordered_map<wstring, void*>* CBase::GetComponentData() const { return _componentData; }

// Component Rectangle Setter/Getter

void CBase::SetComponentSize(Size newSize) {
    auto rect    = (Rect*)((*_componentData)[L"componentRect"]);
    rect->Width  = newSize.Width;
    rect->Height = newSize.Height;
}

void CBase::SetComponentPosition(Point newPosition) {
    auto rect = (Rect*)((*_componentData)[L"componentRect"]);
    rect->X   = newPosition.X;
    rect->Y   = newPosition.Y;
}

Size CBase::GetComponentSize() const {
    auto rect = (Rect*)((*_componentData)[L"componentRect"]);
    Size retSize{};

    rect->GetSize(&retSize);

    return retSize;
}

Point CBase::GetComponentPosition() const {
    auto  rect = (Rect*)((*_componentData)[L"componentRect"]);
    Point retPosition{};

    rect->GetLocation(&retPosition);

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

LRESULT CBase::__Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn) {
    bIsReturn = false;
    return NULL;
}

void CBase::__Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {}
