#include "DebugLibrary.h"

#include "framework.h"

#pragma comment(lib, "../ReleaseOut/lib/UIEngine.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const auto& pUIManager = Engine::UIManager::Initialize(hInstance);

    const auto pWindowMain = pUIManager->CreateCentralWindow(L"Debug Library Window. - Main");
    {
        pWindowMain->ComponentLabel = L"Debug window. Main"s;

        const auto floor1 = new Engine::Component::CBase({});
        
        floor1->ComponentRect = Rect({0, 0, 400, 600});
        floor1->ComponentLabel = L"floor 1 - Main"s;
        pWindowMain->SetChildComponent(floor1);

        const auto floor2 = new Engine::Component::CBase({});

        floor2->ComponentRect = Rect({0, 0, 400, 300});
        floor2->ComponentLabel = L"floor 2 - Main"s;
        floor1->SetChildComponent(floor2);

        const auto floor3 = new Engine::Component::CBase({});

        floor3->ComponentRect = Rect({0, 0, 200, 300});
        floor3->ComponentLabel = L"floor 3 - Main"s;
        floor2->SetChildComponent(floor3);

        ShowWindow(pWindowMain->GetWindowHandle(), nCmdShow);
        UpdateWindow(pWindowMain->GetWindowHandle());
    }

    const auto pWindowSub = pUIManager->CreateCentralWindow(L"Debug Library Window. - Sub", L"UIEngine.Window", {400, 600}, pWindowMain);
    {
        pWindowSub->ComponentLabel = L"Debug window. Sub"s;

        const auto floor1 = new Engine::Component::CBase({});

        floor1->ComponentRect  = Rect({0, 0, 400, 600});
        floor1->ComponentLabel = L"floor 1 - Sub"s;
        pWindowSub->SetChildComponent(floor1);

        const auto floor2 = new Engine::Component::CBase({});

        floor2->ComponentRect  = Rect({0, 0, 400, 300});
        floor2->ComponentLabel = L"floor 2 - Sub"s;
        floor1->SetChildComponent(floor2);

        const auto floor3      = new Engine::Component::CBase({});
        floor3->ComponentRect  = Rect({0, 0, 200, 300});
        floor3->ComponentLabel = L"floor 3 - Sub"s;
        floor2->SetChildComponent(floor3);

        ShowWindow(pWindowSub->GetWindowHandle(), nCmdShow);
        UpdateWindow(pWindowSub->GetWindowHandle());
    }

    return Engine::UIManager::Get().StartMessageLoop();
}
