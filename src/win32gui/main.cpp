#include "msg_logger.hpp"
#include "system.hpp"
#include "win32gui.hpp"

#include <memory>
#include <iostream>
#include <windows.h>

using namespace std ;

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    if(!Win32GUI::is_init(hInstance)) {
        return 0 ;
    }

    if(!System::is_init()) {
        return 0 ;
    }

    MSG msg ;
    while(System::is_update() && Win32GUI::is_update()) {
        //MessageRoop
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;

            DispatchMessage(&msg) ;
        }

        Sleep(5) ; //5ms
    }

    return static_cast<int>(msg.wParam) ;
}