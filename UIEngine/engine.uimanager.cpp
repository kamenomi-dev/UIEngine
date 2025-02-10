#include "pch.h"

#include "engine.h"

using namespace Engine;
using namespace Component;

// UIManager Initialization

UIManager* UIManager::__hInstance{nullptr};

UIManager* UIManager::Initialize(HINSTANCE hInstance) {
    __hInstance = new UIManager(hInstance);

    return __hInstance;
}

UIManager* UIManager::Get() {
    CHECK_RESULT_BOOL(__hInstance);

    return __hInstance;
}

UIManager::UIManager(HINSTANCE hInstance) : _hProcessInstance(hInstance), _windowMap(nullptr), _mainWindowMap(nullptr) {
    this->_windowMap     = new unordered_map<HWND, Component::CWindow*>;
    this->_mainWindowMap = new unordered_map<HWND, Component::CWindow*>;
    this->_windowTreeMap = new unordered_map<HWND, CComponentTree*>;

    Engine::Initialize(hInstance);
}

UIManager::~UIManager() {
    if (_windowMap != nullptr) {
        delete _windowMap;
        _windowMap = nullptr;
    }

    if (_mainWindowMap != nullptr) {
        delete _mainWindowMap;
        _mainWindowMap = nullptr;
    }
}

// UIManager Property Getter/Setter

HINSTANCE UIManager::GetProcessInstance() const { return _hProcessInstance; }

// UIManager Window Creation

CWindow* UIManager::CreateCentralWindow(wstring titleText, wstring className, Size windowSize, CWindow* parentWindow) {
    DEVMODEW devMode{};
    devMode.dmSize = sizeof DEVMODEW;

    EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &devMode);

    const auto screenWidth  = devMode.dmPelsWidth | 1920;  // default?
    const auto screenHeight = devMode.dmPelsHeight | 1080; // default?

    Rect windowRect = {
        (int)std::floor((screenWidth - windowSize.Width) / 2),
        (int)std::floor((screenHeight - windowSize.Height) / 2),
        windowSize.Width,
        windowSize.Height
    };


    return CreateNormalWindow(titleText, className, windowRect, parentWindow);
}

CWindow* UIManager::CreateNormalWindow(wstring titleText, wstring className, Rect windowRect, CWindow* parentWindow) {
    auto newWindow = new CWindow({
        {L"titleText",    (titleText)},
        {L"classText",    (className)},
        {L"windowRect",   Rect(windowRect)  },
        {L"parentWindow", parentWindow          }
    });

    // Default is main for first main.

    if (((_windowMap->at(newWindow->GetWindowHandle()) == nullptr) && _windowMap->size() == 1)
        || _windowMap->size() == 0) {
        newWindow->SetPropertyIfNotExistByValue(L"isOwnerWindow", new bool(true));
    }

    __InsertWindowMap(newWindow->GetWindowHandle(), newWindow);
    return newWindow;
}

void UIManager::__InsertWindowMap(HWND hWnd, Component::CWindow* pWindow) {
    CHECK_RESULT_BOOL(pWindow);

    const auto selfWindowHandle = pWindow->GetWindowHandle();
    if (CProperty_GetProperty_WithInstance(pWindow, L"isOwnerWindow", bool)) {
        (*_mainWindowMap)[selfWindowHandle] = pWindow;
    }

    (*_windowMap)[selfWindowHandle]     = pWindow;
    (*_windowTreeMap)[selfWindowHandle] = new CComponentTree(pWindow);
}

// UIManager Message Processor

LRESULT UIManager::WindowsMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    static auto& pUIManager = *UIManager::Get();
    auto&        pWindow    = (*pUIManager._windowMap)[hWnd];
    auto         bNoop      = false;

    static Point   ptMouse{};
    static wstring hoveredCompLabel{};

    if (uMsg == WM_MOUSEMOVE) {
        ptMouse = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        auto result = (*pUIManager._windowTreeMap)[hWnd]->TryHitTest(ptMouse);

        const auto finalWindow = result.at(0);
        if (finalWindow != nullptr) {
            hoveredCompLabel = finalWindow->GetComponentLabel();
        }

        RECT rect{};
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = 200;
        rect.bottom = 32;
        InvalidateRect(hWnd, &rect, 1);

        rect.left   = ptMouse.X - 4;
        rect.top    = ptMouse.Y - 8;
        rect.right  = ptMouse.X + 4;
        rect.bottom = ptMouse.Y + 8;
        InvalidateRect(hWnd, &rect, 1);
    }

    if (uMsg == WM_CLOSE) {
        auto& pMainWindowMap = *(pUIManager._mainWindowMap);

        if (pMainWindowMap.find(hWnd) != pMainWindowMap.end()) {
            pMainWindowMap.erase(hWnd);
        }

        if (pMainWindowMap.size() == 0) {
            PostQuitMessage(NULL);
        }

        return NULL;
    }

    if (uMsg == WM_WINDOWPOSCHANGED) {
        WINDOWPOS windowPos = *(WINDOWPOS*)(void*)lParam;

        if ((windowPos.flags & SWP_NOSIZE) == NULL) {
            pWindow->SetWindowSize({windowPos.cx, windowPos.cy}, true);
        }

        if ((windowPos.flags & SWP_NOMOVE) == NULL) {
            pWindow->SetWindowPosition({windowPos.x, windowPos.y}, true);
        }

        return NULL; // Skip.
    }

    // Todo, how?
    if (uMsg == WM_PAINT) {
        auto        swapBuffer = pWindow->GetWindowSwapBuffer();
        PAINTSTRUCT paintStruct{};

        auto hTargetDC = BeginPaint(hWnd, &paintStruct);

        Gdiplus::Graphics graphics(swapBuffer->GetRenderableDC());

        paintStruct.rcPaint;

        
        pWindow->Render(graphics);

        Gdiplus::Font       font(L"Segoe UI", 16);
        Gdiplus::SolidBrush brush(Gdiplus::Color::NavajoWhite);
        Gdiplus::SolidBrush mouseBrush(Gdiplus::Color::DarkGray);

        graphics.DrawString(hoveredCompLabel.c_str(), hoveredCompLabel.length(), &font, {0, 0}, &brush);
        graphics.FillRectangle(&mouseBrush, Rect({ptMouse.X - 4, ptMouse.Y - 8, 8, 16}));
        

        swapBuffer->Present(hTargetDC);
        EndPaint(hWnd, &paintStruct);
        return NULL;
    }

    if (uMsg == WM_ERASEBKGND) {
        return TRUE;
    }

    LRESULT dwmResult{};
    if (DwmDefWindowProc(hWnd, uMsg, wParam, lParam, &dwmResult)) {
        return dwmResult;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

WPARAM UIManager::StartMessageLoop() {
    MSG msgStruct{};

    while (GetMessage(&msgStruct, nullptr, 0, 0)) {
        TranslateMessage(&msgStruct);
        DispatchMessage(&msgStruct);
    }

    Uninitialize();

    return msgStruct.wParam;
}

// Component Tree

CComponentTree::CComponentTree(CWindow* pRootWindow) : _pRootWindow(pRootWindow) {}

CWindow* CComponentTree::GetRootWindow() const { return _pRootWindow; }

// Compenont Tree - -  Hittest Implement

inline static void __TryHitTestConditionNext(
    LPVOID                 lParam,
    TTryHittestCondition&& conditionFunc,
    vector<CBase*>*        components,
    vector<CBase*>*        resultComponents
) {
    for (size_t idx = 0; idx < components->size(); idx++) {
        auto currComp = (*components)[idx];
        if (currComp == nullptr) {
            continue;
        }

        auto resultComp = conditionFunc(lParam, currComp, currComp->GetChildCompnents(), resultComponents);

        if (resultComp != nullptr) {
            resultComponents->push_back(resultComp);
        }
    }
};

inline static CBase* __TryHitTestFromRect(
    LPVOID          targetRect,
    CBase*          currComp,
    vector<CBase*>* currCompChildren,
    vector<CBase*>* resultComponents
) {
    auto currentRect = CProperty_GetProperty_WithInstance(currComp, L"componentRect", Rect);

    if (((Rect*)targetRect)->Contains(currentRect)) {

        __TryHitTestConditionNext(targetRect, __TryHitTestFromRect, currCompChildren, resultComponents);
        return currComp;
    }

    return nullptr;
}

inline static CBase* __TryHitTestFromPoint(
    LPVOID          targetPoint,
    CBase*          currComp,
    vector<CBase*>* currCompChildren,
    vector<CBase*>* resultComponents
) {
    auto& currentRect = currComp->GetPropertyTyped<Rect>(L"componentRect");

    if (currentRect.Contains(*(Point*)targetPoint)) {

        __TryHitTestConditionNext(targetPoint, __TryHitTestFromPoint, currCompChildren, resultComponents);
        return currComp;
    }

    return nullptr;
}

vector<CBase*> CComponentTree::TryHitTest(Rect targetRect) {
    return TryHitTestWithCondition(__TryHitTestFromRect, &targetRect);
}

vector<CBase*> CComponentTree::TryHitTest(Point targetPoint) {
    return TryHitTestWithCondition(__TryHitTestFromPoint, &targetPoint);
}

vector<CBase*> CComponentTree::TryHitTestWithCondition(TTryHittestCondition conditionFunc, LPVOID lParam) {
    vector<CBase*> resultComps{};

    resultComps.push_back(conditionFunc(lParam, _pRootWindow, _pRootWindow->GetChildCompnents(), &resultComps));

    return resultComps;
}
