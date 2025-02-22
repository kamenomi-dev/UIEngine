#include "DebugLibrary.h"

#include "framework.h"

#pragma comment(lib, "../ReleaseOut/lib/UIEngine.lib")

using namespace Engine::Component;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const auto& pUIManager = Engine::UIManager::Initialize(hInstance).get();

    auto ownerWindow = new Window();
    ownerWindow->SetStatusFlag({ComponentStatusFlags::Visible});
    ownerWindow->SetFrameFlag({WindowFrameFlag::Central});
    ownerWindow->SetFrameFlag({WindowFrameFlag::Borderless});

    pUIManager->InitializeWindow({ownerWindow});

    return Engine::UIManager::Get().StartMessageLoop();
}
