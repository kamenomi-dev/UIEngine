#include "DebugLibrary.h"

#include "framework.h"

#pragma comment(lib, "../ReleaseOut/lib/UIEngine.lib")

using namespace Engine::Components;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Engine::Initialize(hInstance);

    auto ownerWindow = new Window();
    ownerWindow->SetStatusFlag({ComponentStatusFlags::Visible});
    ownerWindow->SetFrameFlag({WindowFrameFlags::Central});
    // ownerWindow->SetFrameFlag({WindowFrameFlags::Borderless});

    Engine::UIManager::Get().InitializeWindow({ownerWindow});

    return Engine::StartMessageLoop();
}
