#include "pch.h"

#include "engine.h"

/*
Defines::__structWindowProps::__structWindowProps() {
    this->titleText = L"";
    this->className = L"";
    this->disabled  = false;
    this->visibled  = false;
    this->wndRect   = {};
}

Defines::__structWindowProps::__structWindowProps(const __structWindowProps& newProps) {
    this->titleText = newProps.titleText;
    this->className = newProps.className;
    this->disabled  = newProps.disabled;
    this->visibled  = newProps.visibled;
    this->wndRect   = newProps.wndRect;
}

Defines::__structWindowData::__structWindowData() : __structWindowProps() {
    this->isMainWindow = false;
    this->hWnd          = NULL;
    this->hParentWindow = NULL;
}

Defines::__structWindowData::__structWindowData(const __structWindowData& newData) {
    this->isMainWindow  = false;
    this->hWnd          = newData.hWnd;
    this->hParentWindow = newData.hParentWindow;

    this->titleText = newData.titleText;
    this->className = newData.className;
    this->disabled  = newData.disabled;
    this->visibled  = newData.visibled;
    this->wndRect   = newData.wndRect;
}

Defines::__structWindowData::__structWindowData(HWND hParentWindow, __structWindowProps props) {
    __structWindowProps();
    this->hWnd          = NULL;
    this->hParentWindow = hParentWindow;

    this->isMainWindow = hParentWindow == NULL;
    this->titleText    = props.titleText;
    this->className    = props.className;
    this->disabled     = props.disabled;
    this->visibled     = props.visibled;
    this->wndRect      = Rect(props.wndRect);
}

CWindow::CWindow(HWND hParentWindow, IWindowProps props)
: _windowData(new WindowData(hParentWindow, props)) {
    __InitializeWindow();
}

CWindow::~CWindow() {
    if (_windowData != nullptr) {
        delete _windowData;
        _windowData = nullptr;
    }
}

void CWindow::SetAsMainWindow() {
    this->_windowData->isMainWindow  = true;
    this->_windowData->hParentWindow = NULL;
}

bool CWindow::IsMainWindow() const { return _windowData->isMainWindow; }

WindowData CWindow::GetWindowData() const { return *_windowData; }

HWND CWindow::__InitializeWindow() {
    const auto uiManager             = UIManager::Get();
    const auto processInstanceHandle = uiManager->GetProcessInstance();

    WNDCLASSEXW classInfo{NULL};

    classInfo.cbSize        = sizeof WNDCLASSEXW;
    classInfo.hInstance     = processInstanceHandle;
    classInfo.lpfnWndProc   = (WNDPROC)&UIManager::WindowsMessageProcessor;
    classInfo.lpszClassName = _windowData->className;

    // Whatever the class has been registered or not.
    RegisterClassExW(&classInfo);

    const auto newWindow = CreateWindowExW(
        WS_EX_APPWINDOW,
        _windowData->className,
        _windowData->titleText,
        WS_OVERLAPPEDWINDOW,
        _windowData->wndRect.X,
        _windowData->wndRect.Y,
        _windowData->wndRect.Width,
        _windowData->wndRect.Height,
        _windowData->hParentWindow,
        NULL,
        processInstanceHandle,
        nullptr // maybe?
    );

    CHECK_RESULT_BOOL(newWindow);

    return _windowData->hWnd = newWindow;
}

*/