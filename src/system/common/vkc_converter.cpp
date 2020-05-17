#include "vkc_converter.hpp"

#include "virtual_key_fwd.hpp"
#include "msg_logger.hpp"

#include <unordered_map>
#include <utility>
#include <windows.h>
#include <iostream>

using namespace std ;

namespace VKCConverter
{
    inline static const auto& _printable_ascii() noexcept {
        static const auto data = {
            ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')',
            '*', '+', ',', '-', '.', '/', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', ':', ';', '<', '=',
            '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
            'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[',
            '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e',
            'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
            'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
            'z', '{', '|', '}', '~'
        } ;

        return data ;
    }

    //not use boost::bimaps in order to be efficient
    //unorderd_map is O(1)
    static unordered_map<char, unsigned char> char2vkc{} ;
    static unordered_map<unsigned char, char> vkc2char{} ;
    static unordered_map<char, unsigned char> shifted_char2vkc{} ;
    static unordered_map<unsigned char, char> shifted_vkc2char{} ;

    void load_input_combination() noexcept {
        char2vkc.clear() ;
        vkc2char.clear() ;
        shifted_char2vkc.clear() ;
        shifted_vkc2char.clear() ;

        for(const auto c : _printable_ascii()) {
            const auto res = VkKeyScanA(c) ;

            const auto vkc = static_cast<unsigned char>(res & 0xff) ;
            const auto shifted = (res & 0x0100) != 0 ;

            if(shifted) {
                shifted_char2vkc.insert(make_pair(c, vkc)) ;
                shifted_vkc2char.insert(make_pair(vkc, c)) ;
            }
            else {
                char2vkc.insert(make_pair(c, vkc)) ;
                vkc2char.insert(make_pair(vkc, c)) ;
            }
        }
    }

    unsigned char get_vkc(const char ascii) noexcept {
        try{return char2vkc.at(ascii) ;}
        catch(const out_of_range& e) {
            return 0 ;
        }
    }

    char get_ascii(const unsigned char vkc) noexcept {
        try{return vkc2char.at(vkc) ;}
        catch(const out_of_range& e) {
            return 0 ;
        }
    }

    unsigned char get_shifted_vkc(const char ascii) noexcept {
        try{return shifted_char2vkc.at(ascii) ;}
        catch(const out_of_range& e) {
            return 0 ;
        }
    }

    char get_shifted_ascii(const unsigned char vkc) noexcept {
        try{return shifted_vkc2char.at(vkc) ;}
        catch(const out_of_range& e) {
            return 0 ;
        }
    }

    inline static const auto& _sys_str2vkc() {
        static const unordered_multimap<string, unsigned char> data {
            {"IME",         VKC_FROM_EN},
            {"IME",         VKC_TO_JP},
            {"Tab",         VKC_TAB},
            {"Enter",       VKC_ENTER},
            {"CapsLock",    VKC_CAPS_LOCK},
            {"Left",        VKC_LEFT},
            {"Right",       VKC_RIGHT},
            {"Up",          VKC_UP},
            {"Down",        VKC_DOWN},
            {"BkSpace",     VKC_BKSPACE},

            {"Shift",       VKC_LSHIFT},
            {"Shift",       VKC_RSHIFT},
            {"LShift",      VKC_LSHIFT},
            {"RShift",      VKC_RSHIFT},

            {"Ctrl",        VKC_LCTRL},
            {"Ctrl",        VKC_RCTRL},
            {"LCtrl",       VKC_LCTRL},
            {"RCtrl",       VKC_RCTRL},

            {"Win",         VKC_LWIN},
            {"Win",         VKC_RWIN},
            {"LWin",        VKC_LWIN},
            {"RWin",        VKC_RWIN},

            {"Alt",         VKC_LALT},
            {"Alt",         VKC_RALT},
            {"LAlt",        VKC_LALT},
            {"RAlt",        VKC_RALT},

            {"NoConvert",   VKC_NOCONVERT},
            {"Convert",     VKC_CONVERT},
            {"Kana",        VKC_KANA},
            {"App",         VKC_APP},
            {"Esc",         VKC_ESC},

            {"F1",          VKC_F1},
            {"F2",          VKC_F2},
            {"F3",          VKC_F3},
            {"F4",          VKC_F4},
            {"F5",          VKC_F5},
            {"F6",          VKC_F6},
            {"F7",          VKC_F7},
            {"F8",          VKC_F8},
            {"F9",          VKC_F9},
            {"F10",         VKC_F10},
            {"F11",         VKC_F11},
            {"F12",         VKC_F12},
            {"F13",         VKC_F13},
            {"F14",         VKC_F14},
            {"F15",         VKC_F15},
            {"F16",         VKC_F16},
            {"F17",         VKC_F17},
            {"F18",         VKC_F18},
            {"F19",         VKC_F19},
            {"F20",         VKC_F20},
            {"F21",         VKC_F21},
            {"F22",         VKC_F22},
            {"F23",         VKC_F23},
            {"F24",         VKC_F24},

            {"SnapShot",    VKC_SNAPSHOT},
            {"Scroll",      VKC_SCROLL},
            {"Pause",       VKC_PAUSE},
            {"Insert",      VKC_INSERT},
            {"Home",        VKC_HOME},
            {"PageUp",      VKC_PAGEUP},
            {"Delete",      VKC_DELETE},
            {"End",         VKC_END},
            {"PageDown",    VKC_PAGEDOWN},

            {"NumLock",     VKC_NUMLOCK}
        } ;

        return data ;
    }

    const vector<unsigned char> get_sys_vkc(const string& strkey) noexcept {
        if(_sys_str2vkc().find(strkey) == _sys_str2vkc().end()) {
            return vector<unsigned char>{} ;
        }

        vector<unsigned char> vec ;
        const auto ret = _sys_str2vkc().equal_range(strkey) ;
        try {
            for(auto it = ret.first ; it != ret.second ; it++) {
                vec.push_back(it->second) ;
            }
            return vec ;
        }
        catch(const bad_alloc& e) {
            Logger::error_stream << "[Error] " << e.what() << " (vkc_converter.cpp::VKCConverter::get_sys_vkc)\n" ;
            return vec ;
        }
    }

    const vector<unsigned char> get_all_sys_vkc() {
        vector<unsigned char> vec ;
        for(const auto& elem : _sys_str2vkc()) {
            vec.push_back(elem.second) ;
        }

        return vec ;
    }
}