#include "DebugLibrary.h"

#include "framework.h"

#pragma comment(lib, "../ReleaseOut/lib/UIEngine.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const auto pUIManager = Engine::UIManager::Initialize(hInstance);

    const auto pWindowMain = pUIManager->CreateCentralWindow(L"Debug Library Window. - Main");
    {
        pWindowMain->SetComponentLabel(L"Debug window. Main");

        const auto floor1 = new Engine::Component::CBase({
            {L"componentRect", new Rect({0, 0, 400, 600})}
        });
        floor1->SetComponentLabel(L"floor 1 - Main");
        pWindowMain->SetChildComponent(floor1);

        const auto floor2 = new Engine::Component::CBase({
            {L"componentRect", new Rect({0, 0, 400, 300})}
        });
        floor2->SetComponentLabel(L"floor 2 - Main");
        floor1->SetChildComponent(floor2);

        const auto floor3 = new Engine::Component::CBase({
            {L"componentRect", new Rect({0, 0, 200, 300})}
        });
        floor3->SetComponentLabel(L"floor 3 - Main");
        floor2->SetChildComponent(floor3);

        ShowWindow(pWindowMain->GetWindowHandle(), nCmdShow);
        UpdateWindow(pWindowMain->GetWindowHandle());
    }

    const auto pWindowSub = pUIManager->CreateCentralWindow(L"Debug Library Window. - Sub", L"UIEngine.Window", {400, 600}, pWindowMain);
    {
        pWindowSub->SetComponentLabel(L"Debug window. Sub");

        const auto floor1 = new Engine::Component::CBase({
            {L"componentRect", new Rect({0, 0, 400, 600})}
        });
        floor1->SetComponentLabel(L"floor 1 - Sub");
        pWindowSub->SetChildComponent(floor1);

        const auto floor2 = new Engine::Component::CBase({
            {L"componentRect", new Rect({0, 0, 400, 300})}
        });
        floor2->SetComponentLabel(L"floor 2 - Sub");
        floor1->SetChildComponent(floor2);

        const auto floor3 = new Engine::Component::CBase({
            {L"componentRect", new Rect({0, 0, 200, 300})}
        });
        floor3->SetComponentLabel(L"floor 3 - Sub");
        floor2->SetChildComponent(floor3);

        ShowWindow(pWindowSub->GetWindowHandle(), nCmdShow);
        UpdateWindow(pWindowSub->GetWindowHandle());
    }

    return (int)Engine::UIManager::Get()->StartMessageLoop();
}

/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static bool                         isDown = false;
    static Gdiplus::Point               downPos{0, 0};
    static CSwapBuffer* swapBuffer;

    switch (message) {
    case WM_CREATE: {
        swapBuffer = new CSwapBuffer(hWnd);
        break;
    }
    case WM_SIZE: {
        swapBuffer->RefreshSize();
        break;
    }
    case WM_LBUTTONDOWN: {
        isDown  = true;
        downPos = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        break;
    }
    case WM_LBUTTONUP: {
        isDown = false;
        break;
    }
    case WM_MOUSEMOVE: {
        const int         x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
        Gdiplus::Graphics grap(swapBuffer->GetRenderableDC());
        Gdiplus::Pen      pen(Gdiplus::Color::Yellow);

        grap.Clear(Gdiplus::Color::Transparent);

        if (isDown) {
            grap.DrawRectangle(&pen, Gdiplus::Rect{downPos, Gdiplus::Size(x - downPos.X, y - downPos.Y)});
        } else {
            auto ret = grap.DrawRectangle(&pen, Gdiplus::Rect{x - 5, y - 10, 10, 20});
            OutputDebugStringA(std::to_string(ret).c_str());
        }
        swapBuffer->Present();
        break;
    }
    case WM_DESTROY: {
        delete swapBuffer;
        swapBuffer = nullptr;
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
*/