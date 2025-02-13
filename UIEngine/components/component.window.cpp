#include "pch.h"

#include "../engine.h"

using Engine::Render::SwapBuffer;
using namespace Engine::Logic;
using namespace Engine::Component;


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

HWND CWindow::__CreateWindow() {
    const auto& rect    = GetPropertyTyped<Rect>(L"windowRect");
    const auto& pParent = GetPropertyTyped<CWindow*>(L"parentWindow");

    if (pParent == nullptr) {
        SetPropertyByValue(L"isOwnerWindow", true);
    }

    HWND hParent{pParent ? pParent->GetWindowHandle() : nullptr};

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
        nullptr,
        UIManager::Get().GetProcessInstance(),
        nullptr
    );


    const auto err = GetLastError();
    CHECK_RESULT_BOOL(_hSelfWindow = hWnd);

    return hWnd;
}

void CWindow::Render(Gdiplus::Graphics& grap) { CBase::Render(grap); }

LRESULT
inline CWindow::_Native_ComponentMessageProcessor(
    HWND   hWnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam,
    bool&  bIsReturn
) {
    if (uMsg == WM_PAINT) {
        PAINTSTRUCT paintStruct{};
        const HDC   hTargetDC = ::BeginPaint(hWnd, &paintStruct);
        const auto& paintRect = paintStruct.rcPaint;

        auto&                   swapBuffer = GetWindowSwapBuffer();
        const Gdiplus::Graphics graphics{swapBuffer.GetRenderableDC()};

        const Gdiplus::Rect renderableRect{
            paintRect.left,
            paintRect.top,
            paintRect.right - paintRect.left,
            paintRect.bottom - paintRect.top
        };

        auto coveredComponents = _componentTree->TryHitTest(renderableRect) | views::reverse
                               | views::filter([](const auto& p) { return p != nullptr; });

        for (const auto& pComponent : coveredComponents) {
            pComponent->_Native_TransformMessageProcessor(CM_PAINT, 0, (LPARAM)&graphics);
        }

        swapBuffer.Present(hTargetDC);

        ::EndPaint(hWnd, &paintStruct);
    }
    return 0;
}