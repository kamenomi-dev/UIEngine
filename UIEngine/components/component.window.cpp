#include "pch.h"

#include "../engine.h"

using namespace Engine::Component;

// Component Initialization

CWindow::CWindow(vector<PropertyPair> pairs) : CBase(pairs) {
    _componentData->emplace(L"titleText", new wstring(L"Window."));
    _componentData->emplace(L"classText", new wstring(L"UIEngine.Window"));
    _componentData->emplace(L"windowRect", new Rect());
    _componentData->emplace(L"parentWindow", nullptr);
    _componentData->emplace(L"isOwnerWindow", new bool(false));

    __RegisterClass();

    const auto rect = *(Rect*)GetProperty(L"windowRect");
    SetComponentSize({rect.Width, rect.Height});

    const auto pParent = (CWindow*)GetProperty(L"parentWindow");
    HWND       hParent = NULL;
    if (pParent != nullptr) {
        hParent = pParent->GetWindowHandle();
    }


    const auto hWnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        ((wstring*)GetProperty(L"classText"))->c_str(),
        ((wstring*)GetProperty(L"titleText"))->c_str(),
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
    classInfo.lpfnWndProc   = (WNDPROC)&UIManager::WindowsMessageProcessor;
    classInfo.lpszClassName = ((wstring*)GetProperty(L"classText"))->c_str();

    (RegisterClassExW(&classInfo));
    const auto err = GetLastError();
    _wndClassInfo  = classInfo;
}

void CWindow::__UnregisterClass() { (RegisterClassExW(&_wndClassInfo)); }

// Component Property Getter/Setter


wstring CWindow::GetComponentClass() const { return L"Window"; }

bool CWindow::IsOwnerWindow() const { return *(bool*)GetProperty(L"isOwnerWindow"); }

HWND CWindow::GetWindowHandle() const {
    CHECK_RESULT_BOOL(_hSelfWindow);
    return _hSelfWindow;
}

void CWindow::SetWindowOwner(CWindow* pOwner) {
    SetProperty(L"parentWindow", pOwner);
    SetProperty(L"isOwnerWindow", new bool(false));
}

// Component Rectangle Getter/Setter

void CWindow::SetWindowSize(Size newSize, bool isNative) {
    auto winRect  = (Rect*)((*_componentData)[L"windowRect"]);
    auto compRect = (Rect*)((*_componentData)[L"componentRect"]);

    winRect->Width = compRect->Width = newSize.Width;
    winRect->Height = compRect->Height = newSize.Height;

    __UpdateRectangle(isNative);
}

void CWindow::SetWindowPosition(Point newPosition, bool isNative) {
    auto winRect = (Rect*)((*_componentData)[L"windowRect"]);

    winRect->X = newPosition.X;
    winRect->Y = newPosition.Y;

    __UpdateRectangle(isNative);
}

Size CWindow::GetWindowSize() const {
    auto rect = (Rect*)((*_componentData)[L"windowRect"]);
    Size retSize{};

    rect->GetSize(&retSize);

    return retSize;
}

Point CWindow::GetWindowPosition() const {
    auto  rect = (Rect*)((*_componentData)[L"windowRect"]);
    Point retPosition{};

    rect->GetLocation(&retPosition);

    return retPosition;
}

void CWindow::__UpdateRectangle(bool isNative) {
    static Rect lastRect{};
    auto&       currRect = *(Rect*)((*_componentData)[L"componentRect"]);


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