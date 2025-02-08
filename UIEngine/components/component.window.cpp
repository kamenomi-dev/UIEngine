#include "pch.h"

#include "../engine.h"

using namespace Engine::Component;

// Component Initialization

CWindow::CWindow(vector<Utils::PropertyPair> pairs) : CBase(pairs) {
    SetPropertyByValue(L"titleText", L"Window."s);
    SetPropertyByValue(L"classText", L"UIEngine.Window"s);
    SetPropertyByValue(L"windowRect", Rect(0, 0, 800, 600));
    SetPropertyByValue(L"parentWindow", nullptr);
    SetPropertyByValue(L"isOwnerWindow", false);

    SetPropertyByValue(L"componentRect", GetPropertyTyped<Rect>(L"windowRect"));

    auto a = GetPropertyTyped<wstring>(L"classText");

    __RegisterClass();

    const auto rect = CProperty_GetProperty(L"windowRect", Rect);
    SetComponentSize({rect.Width, rect.Height});

    const auto pParent = CProperty_GetProperty(L"parentWindow", CWindow*);
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
    classInfo.lpszClassName = CProperty_GetProperty(L"classText", wstring).c_str();

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
    auto winRect = CProperty_GetProperty(L"windowRect", Rect);
    auto compRect = CProperty_GetProperty(L"componentRect", Rect);

    winRect.Width = compRect.Width = newSize.Width;
    winRect.Height = compRect.Height = newSize.Height;

    SetPropertyByValue(L"windowRect", winRect);
    SetPropertyByValue(L"componentRect", compRect);

    __UpdateRectangle(isNative);
}

void CWindow::SetWindowPosition(Point newPosition, bool isNative) {
    auto winRect = CProperty_GetProperty(L"windowRect", Rect);

    winRect.X = newPosition.X;
    winRect.Y = newPosition.Y;

    SetPropertyByValue(L"windowRect", winRect);

    __UpdateRectangle(isNative);
}

Size CWindow::GetWindowSize() const {
    auto rect = CProperty_GetProperty(L"windowRect", Rect);
    Size  retSize{};

    rect.GetSize(&retSize);

    return retSize;
}

Point CWindow::GetWindowPosition() const {
    auto rect = CProperty_GetProperty(L"windowRect", Rect);
    Point retPosition{};

    rect.GetLocation(&retPosition);

    return retPosition;
}

void CWindow::__UpdateRectangle(bool isNative) {
    static Rect lastRect{};
    static auto currRect = CProperty_GetProperty(L"componentRect", Rect);

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