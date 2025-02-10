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
    SetPropertyIfNotExistByValue(L"componentLabel", L"UIEngine.Base "s + std::to_wstring(rand() % time(0)));
}

CBase::~CBase() {}

// Component Identity Settger/Getter

wstring CBase::GetComponentClass() const { return L"Base"; }

void CBase::SetComponentLabel(wstring newLabel) { SetPropertyByValue(L"componentLabel", newLabel); }

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

inline Rect CBase::GetComponentRect() const { return GetPropertyTyped<Rect>(L"componentRect"); }

inline Size CBase::GetComponentSize() const {
    const auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");
    Size        returnSize{};

    componentRect.GetSize(&returnSize);

    return returnSize;
}

inline Point CBase::GetComponentPosition() const {
    const auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");
    Point       returnPosition{};

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

void CBase::Render(Gdiplus::Graphics& graphics) {
    static unordered_map<void*, Gdiplus::Color> map{};

    if (map.find(this) == map.end()) {
        map[this] = Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
    }

    graphics.Clear(map[this]);
}

LRESULT CBase::__Native_ComponentMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn) {
    bIsReturn = false;
    return NULL;
}

void CBase::__Native_TransformMessageProcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == ComponentBaseMessageEnum::CM_BASE_PAINT) {
        auto& graphics = *((unique_ptr<Gdiplus::Graphics>*)lParam)->get();

        const auto lastState = graphics.Save();
        {
            graphics.SetClip(GetComponentRect());
            Render(graphics);
        }
        graphics.Restore(lastState);
    }
}
