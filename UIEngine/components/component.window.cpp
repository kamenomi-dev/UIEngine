#include "pch.h"

#include "../engine.h"

using namespace Engine::Component;

// Component Initialization

CWindow::CWindow(vector<Utils::PropertyPair> pairs) : CBase(pairs) {
    SetPropertyByValue(L"componentLabel", L"UIEngine.Window"s);

    SetPropertyIfNotExistByValue(L"titleText", L"Window."s);
    SetPropertyIfNotExistByValue(L"classText", L"UIEngine.Window"s);
    SetPropertyIfNotExistByValue(L"windowRect", make_any<Rect>(0, 0, 800, 600));
    SetPropertyIfNotExistByValue(L"parentWindow", nullptr);
    SetPropertyIfNotExistByValue(L"isOwnerWindow", false);

    auto ptr = &GetPropertyTyped<Rect>(L"windowRect");

    __RegisterClass();

    const auto& rect = GetPropertyTyped<Rect>(L"windowRect");
    SetComponentSize({rect.Width, rect.Height});

    const auto pParent = any_cast<CWindow*>(GetProperty(L"parentWindow"));
    HWND       hParent = NULL;
    if (pParent != nullptr) {
        hParent = pParent->GetWindowHandle();
    }

    const auto hWnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        CProperty_GetProperty(L"classText", wstring).c_str(),
        CProperty_GetProperty(L"titleText", wstring).c_str(),
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
    _renderSwapBuffer = new Render::CSwapBuffer(hWnd);
}

CWindow::~CWindow() {
    __UnregisterClass();

    if (_renderSwapBuffer != nullptr) {
        delete _renderSwapBuffer;
        _renderSwapBuffer = nullptr;
    }
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

// Component Property Getter/Setter


wstring CWindow::GetComponentClass() const { return L"Window"; }

bool CWindow::IsOwnerWindow() const { return CProperty_GetProperty(L"isOwnerWindow", bool); }

HWND CWindow::GetWindowHandle() const {
    CHECK_RESULT_BOOL(_hSelfWindow);
    return _hSelfWindow;
}

void CWindow::SetWindowOwner(CWindow* pOwner) {
    SetPropertyByValue(L"parentWindow", pOwner);
    SetPropertyByValue(L"isOwnerWindow", false);
}

// Component Rectangle Getter/Setter

void CWindow::SetWindowSize(Size newSize, bool isNative) {
    auto& windowRect    = GetPropertyTyped<Rect>(L"windowRect");
    auto& componentRect = GetPropertyTyped<Rect>(L"componentRect");

    windowRect.Width = componentRect.Width = newSize.Width;
    windowRect.Height = componentRect.Height = newSize.Height;

    __UpdateRectangle(isNative);
}

void CWindow::SetWindowPosition(Point newPosition, bool isNative) {
    auto& windowRect = GetPropertyTyped<Rect>(L"windowRect");

    windowRect.X = newPosition.X;
    windowRect.Y = newPosition.Y;

    __UpdateRectangle(isNative);
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

void CWindow::__UpdateRectangle(bool isNative) {
    static Rect lastRect{};
    auto&       currRect = GetPropertyTyped<Rect>(L"componentRect");

    if (not lastRect.Equals(currRect)) {

        if (lastRect.Width != currRect.Width || lastRect.Height != currRect.Height) {
            _renderSwapBuffer->RefreshSize();
        }

        if (not isNative) {
            SetWindowPos(
                _hSelfWindow,
                NULL,
                currRect.X,
                currRect.Y,
                currRect.Width,
                currRect.Height,
                SWP_NOOWNERZORDER
            );
        }

        lastRect = currRect;
    }
}

// Component Render -- How?

Engine::Render::CSwapBuffer* CWindow::GetWindowSwapBuffer() { return _renderSwapBuffer; }

void CWindow::Render(Gdiplus::Graphics& grap) {
    static auto bgColor = Gdiplus::Color(3, 3, 15);
    grap.Clear(bgColor);
}

// Component Message Processor

LRESULT
CWindow::__Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIsReturn) {

    return NULL;
}