#pragma once
#ifndef __ENGINE_UIMANAGER_H__
#define __ENGINE_UIMANAGER_H__

namespace Engine {

namespace Defines {

typedef Component::CBase*(TTryHittestCondition)(LPVOID,
                                                Component::CBase*,
                                                vector<Component::CBase*>*,
                                                vector<Component::CBase*>*);
typedef void(TTryHittestConditionNext)(LPVOID, TTryHittestCondition, vector<Component::CBase*>*, vector<Component::CBase*>*);

} // namespace Defines

using namespace Defines;

class CComponentTree;

class UIENGINE_API UIManager final {
public:
    static UIManager* Initialize(HINSTANCE hInstance);
    static UIManager* Get();

    ~UIManager();

    HINSTANCE GetProcessInstance() const;

    Component::CWindow* CreateCentralWindow(
        wstring             titleText,
        wstring             className    = L"UIEngine.Window",
        Size                windowSize   = {800, 600},
        Component::CWindow* parentWindow = nullptr
    );
    Component::CWindow* CreateNormalWindow(
        wstring             titleText,
        wstring             className    = L"UIEngine.Window",
        Rect                windowRect   = {0, 0, 800, 600},
        Component::CWindow* parentWindow = nullptr
    );

    static LRESULT WindowsMessageProcessor(HWND, UINT, WPARAM, LPARAM);
    WPARAM         StartMessageLoop();

private:
    static UIManager*                         __hInstance;

    HINSTANCE                                 _hProcessInstance;
    unordered_map<HWND, Component::CWindow*>* _windowMap;// TODO: ��Ϊ��ͨ�ֶ�
    unordered_map<HWND, Component::CWindow*>* _mainWindowMap;
    unordered_map<HWND, CComponentTree*>*     _windowTreeMap;

    UIManager(HINSTANCE hInstance);

    void __InsertWindowMap(HWND, Component::CWindow*);
};

class UIENGINE_API CComponentTree final{
public:
    CComponentTree(Component::CWindow* pRootWindow);

    Component::CWindow*       GetRootWindow() const;
    vector<Component::CBase*> TryHitTest(Rect);
    vector<Component::CBase*> TryHitTest(Point);
    vector<Component::CBase*> TryHitTestWithCondition(TTryHittestCondition, LPVOID lParam);

private:
    Component::CWindow* _pRootWindow;
};
} // namespace Engine

#endif // !__ENGINE_UIMANAGER_H__
