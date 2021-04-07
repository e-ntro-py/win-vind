#include "bind/ctrl/mywindow_ctrl.hpp"

#include <windows.h>

#include "bind/mouse/jump_actwin.hpp"
#include "util/def.hpp"

namespace
{
    std::function<void()> show_func = [] {return ;} ;
    std::function<void()> exit_func = [] {return ;} ;
}

namespace vind
{
    //ShowConfigWindow
    const std::string ShowConfigWindow::sname() noexcept {
        return "show_config_window" ;
    }
    void ShowConfigWindow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        show_func() ;
        Sleep(50) ; //wait until opened window.
        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }
    void ShowConfigWindow::register_show_func(std::function<void()> func) noexcept {
        show_func = std::move(func) ;
    }


    //ExitConfigWindow
    const std::string ExitConfigWindow::sname() noexcept {
        return "exit_config_window" ;
    }
    void ExitConfigWindow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        exit_func() ;
    }
    void ExitConfigWindow::register_exit_func(std::function<void()> func) noexcept {
        exit_func = std::move(func) ;
    }
}