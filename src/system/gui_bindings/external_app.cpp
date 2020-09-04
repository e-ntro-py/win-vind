#include "external_app.hpp"

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"
#include "path.hpp"
#include "jump_cursor.hpp"
#include "utility.hpp"

using namespace std ;

namespace ExAppUtility
{
    using mss_t = unordered_map<string, string> ;

    inline static const mss_t _load_proc_list_core() noexcept {
        mss_t map{} ;

        try {
            nlohmann::json j ;
            std::ifstream ifs(Path::SETTINGS()) ;
            ifs >> j ;

            for(const auto& i : j.at("exapps").at("choices")) {
                try {
                    auto&& key = i.at("name").get<std::string>() ;
                    auto&& val = i.at("value").get<std::string>() ;
                    map[key] = val ;
                }
                catch(const std::exception& e) {continue ;}
            }
            return map ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << ", failed loading external app list (" \
            << Path::SETTINGS() <<") (ExAppUtility::_load_proc_list_core)\n" ;
            return map ;
        }
    }

    static auto proc_list{_load_proc_list_core()} ;

    void load_config() noexcept {
        proc_list = _load_proc_list_core() ;
    }

    inline static const auto get_protected_path(const string name) noexcept {
        try {
            const auto& origin = proc_list.at(name) ;
            //is origin path?
            if(origin.find("/") == string::npos) {
                return origin ;
            }
            if(origin.find("\\") == string::npos) {
                return origin ;
            }
            return "\"" + origin + "\"" ;
        }
        catch(const out_of_range& e) {
            ERROR_STREAM << "Not an external-application command : " << name \
            << " (ExAppUtility::get_protected_path)\n" ;
            return string{} ;
        }
    }

    inline static bool create_process(const string path) noexcept
    {
        STARTUPINFOA si ;
        ZeroMemory(&si, sizeof(si)) ;
        si.cb = sizeof(si) ;

        PROCESS_INFORMATION pi ;
        ZeroMemory(&pi, sizeof(pi)) ;

        if(!CreateProcessA(
            NULL, const_cast<LPSTR>(path.c_str()), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, Path::HOME_PATH().c_str(), &si, &pi)) {
            WIN_ERROR_STREAM << "cannot call \"" << path << "\"" \
            << " (ExAppUtility::create_process)\n" ;
            return false ;
        }
        return true ;
    }
}

using namespace ExAppUtility ;


//StartShell
const string StartShell::sname() noexcept
{
    return "start_shell" ;
}

bool StartShell::sprocess(const string UNUSED(cmd))
{
    if(!create_process(get_protected_path("shell"))) {
        return false ;
    }
    //wait until select window by OS.
    Sleep(100) ;
    if(!Jump2ActiveWindow::sprocess(true)) {
        return false ;
    }
    return true ;
}


//StartAnyApp
const string StartAnyApp::sname() noexcept
{
    return "start_any_app" ;
}

bool StartAnyApp::sprocess(const string cmd)
{
    if(!create_process(get_protected_path(cmd.substr(1)))) {
        return false ;
    }
    //wait until select window by OS.
    Sleep(100) ;
    if(!Jump2ActiveWindow::sprocess(true)) {
        return false ;
    }
    return true ;
}