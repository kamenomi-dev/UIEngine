#include "pch.h"

#include "../engine.h"

using Engine::Render::SwapBuffer;
using namespace Engine::Logic;
using namespace Engine::Component;


CWindow::CWindow(vector<Utils::PropertyPair> pairs) : CBase(pairs) {
    SetPropertyIfNotExistByValue(L"titleText", L"Window."s);
    SetPropertyIfNotExistByValue(L"classText", L"UIEngine.Window"s);
    SetPropertyIfNotExistByValue(L"windowRect", make_any<Rect>(0, 0, 800, 600));
    SetPropertyIfNotExistByValue(L"parentWindow", nullptr);
    SetPropertyIfNotExistByValue(L"isOwnerWindow", false);

    _RegisterClass();

    const auto& rect = GetPropertyTyped<Rect>(L"windowRect");
    ComponentSize    = Size({rect.Width, rect.Height});

    const auto& pParent = GetPropertyTyped<CWindow*>(L"parentWindow");
    if (pParent == nullptr) {
        SetPropertyByValue(L"isOwnerWindow", true);
    }

    _componentTree    = make_unique<CComponentTree>(this);
    _renderSwapBuffer = make_unique<SwapBuffer>(_CreateWindow());
}

void CWindow::_RegisterClass() {
    WNDCLASSEXW classInfo{};
    classInfo.cbSize        = sizeof WNDCLASSEXW;
    classInfo.hInstance     = Engine::hModuleInstance;
    classInfo.lpfnWndProc   = &UIManager::WindowsMessageProcessor;
    classInfo.lpszClassName = GetPropertyTyped<wstring>(L"classText").c_str();

    (RegisterClassExW(&classInfo));
    const auto err   = GetLastError();
    _windowClassInfo = classInfo;
}

HWND CWindow::_CreateWindow() {
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
    CHECK_RESULT_BOOL(_windowHandle = hWnd);

    return hWnd;
}

void CWindow::Render(Gdiplus::Graphics& grap) { CBase::Render(grap); }

LRESULT
CWindow::_Native_ComponentMessageProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& isReturn) {

    if (uMsg == WM_PAINT) {
        PAINTSTRUCT paintStruct{};
        const HDC   hTargetDC = ::BeginPaint(hWnd, &paintStruct);
        const auto& paintRect = paintStruct.rcPaint;

        auto&             swapBuffer = GetWindowSwapBuffer();
        Gdiplus::Graphics graphics{swapBuffer.GetRenderableDC()};

        const Gdiplus::Rect renderableRect{
            paintRect.left,
            paintRect.top,
            paintRect.right - paintRect.left,
            paintRect.bottom - paintRect.top
        };

        auto coveredComponents = _componentTree->TryHitTest(renderableRect) | std::views::reverse
                               | std::views::filter([](const auto& p) { return p != nullptr; });

        for (const auto& pComponent : coveredComponents) {
            if (not pComponent->ComponentVisible) {
                continue;
            }

            const auto lastState = graphics.Save();
            {
                graphics.SetClip(pComponent->ComponentRect);
                pComponent->_Native_TransformMessageProcessor(CM_PAINT, 0, (LPARAM)&graphics);
            }
            graphics.Restore(lastState);
        }

        swapBuffer.Present(hTargetDC);
        ::EndPaint(hWnd, &paintStruct);

        return NULL;
    }

    return CBase::_Native_ComponentMessageProcessor(uMsg, wParam, lParam, isReturn);
}