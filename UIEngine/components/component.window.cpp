#include "pch.h"

#include "../engine.h"

using Engine::Render::SwapBuffer;
using namespace Engine::Logic;
using namespace Engine::Component;

// Component Initialization

CWindow::CWindow(vector<Utils::PropertyPair> pairs) : CBase(pairs) {
    SetPropertyByValue(L"componentLabel", L"UIEngine.Window"s);

    SetPropertyIfNotExistByValue(L"titleText", L"Window."s);
    SetPropertyIfNotExistByValue(L"classText", L"UIEngine.Window"s);
    SetPropertyIfNotExistByValue(L"windowRect", make_any<Rect>(0, 0, 800, 600));
    SetPropertyIfNotExistByValue(L"parentWindow", nullptr);
    SetPropertyIfNotExistByValue(L"isOwnerWindow", false);

    __RegisterClass();

    const auto& rect = GetPropertyTyped<Rect>(L"windowRect");
    SetComponentSize({rect.Width, rect.Height});

    const auto& pParent = GetPropertyTyped<CWindow*>(L"parentWindow");
    if (pParent == nullptr) {
        SetPropertyByValue(L"isOwnerWindow", true);
    }

    _componentTree    = make_unique<CComponentTree>(this);
    _renderSwapBuffer = make_unique<SwapBuffer>(__CreateWindow());
}

CWindow::~CWindow() {
    __DeleteWindow();
    __UnregisterClass();
}

void CWindow::__RegisterClass() {
    WNDCLASSEXW classInfo{};
    classInfo.cbSize        = sizeof WNDCLASSEXW;
    classInfo.hInstance     = Engine::hModuleInstance;
    classInfo.lpfnWndProc   = &UIManager::WindowsMessageProcessor;
    classInfo.lpszClassName = GetPropertyTyped<wstring>(L"classText").c_str();

    (RegisterClassExW(&classInfo));
    const auto err = GetLastError();
    _wndClassInfo  = classInfo;
}

void CWindow::__UnregisterClass() { (RegisterClassExW(&_wndClassInfo)); }

HWND CWindow::__CreateWindow() {
    const auto& rect    = GetPropertyTyped<Rect>(L"windowRect");
    const auto& pParent = GetPropertyTyped<CWindow*>(L"parentWindow");

    if (pParent == nullptr) {
        SetPropertyByValue(L"isOwnerWindow", true);
    }

    HWND hParent = NULL;
    if (pParent != nullptr) {
        hParent = pParent->GetWindowHandle();
    }

    const auto hWnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        GetPropertyTyped<wstring>(L"classText").c_str(),
        GetPropertyTyped<wstring>(L"titleText").c_str(),
        WS_OVERLAPPEDWINDOW,
        rect.X,
        rect.Y,
        rect.Width,
        rect.Height,
        hParent,
        NULL,
        UIManager::Get()->GetProcessInstance(),
        nullptr
    );


    const auto err = GetLastError();
    CHECK_RESULT_BOOL(_hSelfWindow = hWnd);

    return hWnd;
}

void CWindow::__DeleteWindow() { CloseWindow(_hSelfWindow); }

// Component Property Getter/Setter

unique_ptr<CComponentTree>& CWindow::GetComponentTree() { return _componentTree; }

wstring CWindow::GetComponentClass() const { return L"Window"; }

bool CWindow::IsOwnerWindow() const { return GetPropertyTyped<bool>(L"isOwnerWindow"); }

HWND CWindow::GetWindowHandle() const {
    CHECK_RESULT_BOOL(_hSelfWindow);
    return _hSelfWindow;
}

void CWindow::SetWindowOwner(CWindow* pOwner) {
    SetPropertyByValue(L"parentWindow", pOwner);
    SetPropertyByValue(L"isOwnerWindow", false);
}

// Component Rectangle Getter/Setter

void CWindow::SetWindowSize(Size newSize) {
    auto& windowRect    = GetPropertyTyped<Rect>(L"windowRect");
    auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

    windowRect.Width = componentRect.Width = newSize.Width;
    windowRect.Height = componentRect.Height = newSize.Height;

    SetWindowPos(
        _hSelfWindow,
        NULL,
        windowRect.X,
        windowRect.Y,
        windowRect.Width,
        windowRect.Height,
        SWP_NOOWNERZORDER
    );
    __UpdateRectangle();
}

void CWindow::SetWindowPosition(Point newPosition) {
    auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

    windowRect.X = newPosition.X;
    windowRect.Y = newPosition.Y;

    SetWindowPos(
        _hSelfWindow,
        NULL,
        windowRect.X,
        windowRect.Y,
        windowRect.Width,
        windowRect.Height,
        SWP_NOOWNERZORDER
    );
    __UpdateRectangle();
}

Size CWindow::GetWindowSize() const {
    auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");
    Size  size{};

    windowRect.GetSize(&size);

    return size;
}

Point CWindow::GetWindowPosition() const {
    auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");
    Point position{};

    windowRect.GetLocation(&position);

    return position;
}

void CWindow::__UpdateRectangle() {
    static Rect lastRect{};
    const auto& currRect = GetPropertyTyped<Rect>(L"componentRect");

    if (not lastRect.Equals(currRect)) {

        if (lastRect.Width != currRect.Width || lastRect.Height != currRect.Height) {
            _renderSwapBuffer->RefreshSize();
        }

        lastRect = currRect;
    }
}

// Component Render -- How?

unique_ptr<SwapBuffer>& CWindow::GetWindowSwapBuffer() { return _renderSwapBuffer; }

void CWindow::Render(Gdiplus::Graphics& grap) {
    //static auto bgColor = Gdiplus::Color(3, 3, 15);
    //grap.Clear(bgColor);
    CBase::Render(grap);
}

void CWindow::__Native_SetWindowSize(Size newSize) {
    auto& windowRect    = GetPropertyTyped<Rect>(L"windowRect");
    auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

    windowRect.Width = componentRect.Width = newSize.Width;
    windowRect.Height = componentRect.Height = newSize.Height;

    __UpdateRectangle();
}

void CWindow::__Native_SetWindowPosition(Point newPosition) {
    auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

    windowRect.X = newPosition.X;
    windowRect.Y = newPosition.Y;

    __UpdateRectangle();
}

// Component Message Processor

LRESULT
inline CWindow::__Native_ComponentMessageProcessor(
    HWND   hWnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam,
    bool&  bIsReturn
) {
    if (uMsg == WM_PAINT) {
        PAINTSTRUCT paintStruct{};

        auto       hTargetDC = BeginPaint(hWnd, &paintStruct);
        const auto paintRect = paintStruct.rcPaint;

        auto& swapBuffer = *GetWindowSwapBuffer().get();
        auto  graphics   = make_unique<Gdiplus::Graphics>(swapBuffer.GetRenderableDC());

        const auto gdipRect =
            Rect(paintRect.left, paintRect.top, paintRect.right - paintRect.left, paintRect.bottom - paintRect.top);

        auto coveredComponents = _componentTree.get()->TryHitTest(gdipRect);
        reverse(coveredComponents.begin(), coveredComponents.end());

        for (auto& pComponent : coveredComponents) {
            if (pComponent == nullptr) {
                continue;
            }

            pComponent
                ->__Native_TransformMessageProcessor(ComponentBaseMessageEnum::CM_BASE_PAINT, NULL, (LPARAM)&graphics);
        }

        swapBuffer.Present(hTargetDC);
        EndPaint(hWnd, &paintStruct);
    }
    return NULL;
}