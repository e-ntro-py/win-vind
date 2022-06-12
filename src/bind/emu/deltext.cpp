#include "deltext.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "smartclipboard.hpp"
#include "textreg.hpp"
#include "textutil.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        //DeleteHighlightText (Visual only)
        DeleteHighlightText::DeleteHighlightText()
        : BindedFuncVoid("delete_highlight_text")
        {}
        void DeleteHighlightText::sprocess() {
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            if(core::get_global_mode_flags() & core::ModeFlags::VISUAL_LINE) {
                set_register_type(RegType::Lines) ;
            }
            else {
                set_register_type(RegType::Chars) ;
            }
            ToEdiNormal::sprocess(false) ;
        }
        void DeleteHighlightText::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void DeleteHighlightText::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //DeleteLine
        struct DeleteLine::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        DeleteLine::DeleteLine()
        : ChangeBaseCreator("delete_line"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteLine::~DeleteLine() noexcept                  = default ;
        DeleteLine::DeleteLine(DeleteLine&&)            = default ;
        DeleteLine& DeleteLine::operator=(DeleteLine&&) = default ;

        void DeleteLine::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;

            igate.pushup(KEYCODE_HOME) ;
            safe_for(count, [&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            }) ;

            Sleep(24) ;

            igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            set_register_type(RegType::Lines) ;
        }
        void DeleteLine::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteLine::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        // DeleteLineUntilEOL
        DeleteLineUntilEOL::DeleteLineUntilEOL()
        : ChangeBaseCreator("delete_line_until_EOL")
        {}

        void DeleteLineUntilEOL::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;

            //delete N - 1 lines under the current line
            safe_for(count - 1, [&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            }) ;

            auto res = get_selected_text([&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            }) ;

            // Some editors have a visible EOL mark in a line.
            if(res.having_EOL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                if(res.str.empty()) {
                    // clear clipboard with null
                    auto hwnd = util::get_foreground_window() ;
                    SmartClipboard scb(hwnd) ;
                    scb.open() ;
                    scb.set("") ;
                    scb.close() ;
                }
            }

            igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            set_register_type(RegType::Chars) ;
        }

        void DeleteLineUntilEOL::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }

        void DeleteLineUntilEOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //DeleteAfter
        struct DeleteAfter::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;


        DeleteAfter::DeleteAfter()
        : ChangeBaseCreator("delete_after"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteAfter::~DeleteAfter() noexcept                   = default ;
        DeleteAfter::DeleteAfter(DeleteAfter&&)            = default ;
        DeleteAfter& DeleteAfter::operator=(DeleteAfter&&) = default ;

        void DeleteAfter::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                safe_for(count, [&igate] {
                        igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                        igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                        set_register_type(RegType::Chars) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_DELETE) ;
                }) ;
            }
        }
        void DeleteAfter::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteAfter::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //DeleteBefore
        struct DeleteBefore::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        DeleteBefore::DeleteBefore()
        : ChangeBaseCreator("delete_before"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteBefore::~DeleteBefore() noexcept                    = default ;
        DeleteBefore::DeleteBefore(DeleteBefore&&)            = default ;
        DeleteBefore& DeleteBefore::operator=(DeleteBefore&&) = default ;

        void DeleteBefore::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                    set_register_type(RegType::Chars) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_BKSPACE) ;
                }) ;
            }
        }
        void DeleteBefore::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteBefore::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
