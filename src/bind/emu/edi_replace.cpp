#include "edi_replace.hpp"

#include <windows.h>

#include "bind/safe_repeater.hpp"
#include "core/char_logger.hpp"
#include "core/entry.hpp"
#include "core/key_absorber.hpp"
#include "core/keycode_def.hpp"
#include "core/keycodecvt.hpp"
#include "core/ntype_logger.hpp"
#include "opt/vcmdline.hpp"
#include "text_analyzer.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace
{
    using namespace vind ;

    inline auto is_shift(unsigned key) noexcept {
        return key == KEYCODE_SHIFT || key == KEYCODE_LSHIFT || key == KEYCODE_RSHIFT ;
    }

    template <typename FuncT>
    inline void loop_for_keymatching(FuncT&& func) {
        //reset keys downed in order to call this function.
        for(auto& key : core::get_pressed_list()) {
            if(is_shift(key)) continue ;
            util::release_keystate(key) ;
        }

        while(core::update_background()) {
            if(core::is_pressed(KEYCODE_ESC)) {
                return ;
            }
            auto log = core::get_pressed_list() ;

            if(!log.is_containing(KEYCODE_SHIFT)) {
                //not shifted
                for(auto& key : log) {
                    //For example, if replace by 'i' and 'i' key is downed,
                    //immediately will call "insert-mode", so release 'i'.
                    util::release_keystate(key) ;

                    if(!core::get_ascii(key)) {
                        continue ;
                    }
                    if(func(key)) {
                        return ;
                    }
                }
            }
            else {
                //shifted
                for(auto& key : log) {
                    if(is_shift(key)) continue ;
                    util::release_keystate(key) ;
                    if(!core::get_shifted_ascii(key)) {
                        continue ;
                    }
                    if(func(key, true)) {
                        return ;
                    }
                }
            }
        }
    }
}


namespace vind
{
    namespace bind
    {
        //EdiNRplaceChar
        ReplaceChar::ReplaceChar()
        : BindedFuncCreator("replace_char")
        {}
        void ReplaceChar::sprocess(unsigned int repeat_num) {
            loop_for_keymatching([repeat_num](const auto& keycodes, bool shifted=false) {

                safe_for(repeat_num, [&keycodes, shifted] {
                    util::pushup(KEYCODE_DELETE) ;

                    if(shifted) util::pushup(KEYCODE_LSHIFT, keycodes) ;
                    else util::pushup(keycodes) ;
                }) ;

                safe_for(repeat_num, [] {
                    util::pushup(KEYCODE_LEFT) ;
                }) ;

                return true ; //terminate looping
            }) ;
        }
        void ReplaceChar::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ReplaceChar::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //ReplaceSequence
        ReplaceSequence::ReplaceSequence()
        : BindedFuncCreator("replace_sequence")
        {}
        void ReplaceSequence::sprocess(unsigned int repeat_num) {
            using util::pushup ;

            opt::VCmdLine::clear() ;
            opt::VCmdLine::print(opt::GeneralMessage("-- EDI REPLACE --")) ;

            std::vector<KeyCode> strs{} ;
            std::vector<bool> shifts{} ;

            loop_for_keymatching([&strs, &shifts](const auto& keycodes, bool shifted=false) {
                pushup(KEYCODE_DELETE) ;
                if(shifted) {
                    pushup(KEYCODE_LSHIFT, keycodes) ;
                    strs.push_back(keycodes) ;
                    shifts.push_back(true) ;
                }
                else {
                    pushup(keycodes) ;
                    strs.push_back(keycodes) ;
                    shifts.push_back(false) ;
                }
                return false ; //continue looping
            }) ;

            safe_for(repeat_num, [&strs, shifts] {
                for(std::size_t stridx = 0 ; stridx < strs.size() ; stridx ++) {
                    pushup(KEYCODE_DELETE) ;
                    if(shifts[stridx]) pushup(KEYCODE_LSHIFT, strs[stridx]) ;
                    else pushup(strs[stridx]) ;
                }
            }) ;

            opt::VCmdLine::reset() ;
            opt::VCmdLine::print(opt::GeneralMessage("-- EDI NORMAL --")) ;
        }
        void ReplaceSequence::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ReplaceSequence::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //SwitchCharCase
        SwitchCharCase::SwitchCharCase()
        : BindedFuncCreator("switch_char_case")
        {}
        void SwitchCharCase::sprocess(unsigned int repeat_num) {
            auto res = get_selected_text([&repeat_num] {
                    safe_for(repeat_num, [] {
                        util::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                    }) ;
                    util::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                }) ;

            for(char c : res.str) {
                if(c >= 'a' && c <= 'z') {
                    util::pushup(KEYCODE_LSHIFT, core::get_keycode(c)) ;
                }
                else if(c >= 'A' && c <= 'Z') {
                    constexpr char delta = 'a' - 'A' ;
                    util::pushup(core::get_keycode(c + delta)) ;
                }
                else {
                    auto keycode = core::get_keycode(c) ;
                    if(keycode) {
                        util::pushup(keycode) ;
                        continue ;
                    }

                    keycode = core::get_shifted_keycode(c) ;
                    if(keycode) {
                        util::pushup(KEYCODE_LSHIFT, keycode) ;
                        continue ;
                    }
                }
            }
        }
        void SwitchCharCase::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void SwitchCharCase::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
