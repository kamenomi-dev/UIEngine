#include "DebugLibrary.h"

#include "framework.h"

#pragma comment(lib, "../ReleaseOut/lib/UIEngine.lib")

using namespace Engine::Components;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Engine::Initialize(hInstance);

    auto ownerWindow = new Window();
    ownerWindow->SetStatusFlag({CompStatus::Visible});
    ownerWindow->SetFrameFlag({WndFrame::Central});
    // ownerWindow->SetFrameFlag({WindowFrameFlags::Borderless});

    auto subWindow = new Window{};
    subWindow->SetStatusFlag({CompStatus::Visible});
    subWindow->SetFrameFlag({WndFrame::Central});
    // subWindow->SetFrameFlag({WindowFrameFlags::Borderless});

    Engine::InitializeWindow({ownerWindow, subWindow});

    return Engine::StartMessageLoop();
}
