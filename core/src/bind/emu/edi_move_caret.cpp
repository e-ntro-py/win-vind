#include "edi_move_caret.hpp"

#include "io/keybrd_eventer.hpp"
#include "key/key_absorber.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "text/simple_text_selecter.hpp"
#include "time/interval_timer.hpp"
#include "time/keystroke_repeater.hpp"
#include "utility.hpp"

namespace
{
    using namespace vind ;

    template <typename T1, typename T2>
    inline void move_caret_with_repeating(
            const bool first_call,
            const unsigned int repeat_num,
            KeyStrokeRepeater& ksr,
            T1&& v_press_proc,
            T2&& n_press_proc)
    {
        using mode::is_edi_visual ;

        if(repeat_num == 1) {
            if(first_call) {
                ksr.reset() ;

                if(is_edi_visual()) v_press_proc() ;
                else n_press_proc() ;

                return ;
            }
            if(!ksr.is_pressed()) {
                return ;
            }

            if(is_edi_visual()) v_press_proc() ;
            else n_press_proc() ;
        }

        if(first_call) {
            if(is_edi_visual()) {
                for(unsigned int i = 0 ; i < repeat_num ; i ++)
                    v_press_proc() ;
            }
            else {
                for(unsigned int i = 0 ; i < repeat_num ; i ++)
                    n_press_proc() ;
            }
            ksr.reset() ;
        }
    }
}


namespace vind
{
    //EdiMoveCaretLeft
    struct EdiMoveCaretLeft::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretLeft::EdiMoveCaretLeft()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretLeft::~EdiMoveCaretLeft() noexcept                    = default ;
    EdiMoveCaretLeft::EdiMoveCaretLeft(EdiMoveCaretLeft&&)            = default ;
    EdiMoveCaretLeft& EdiMoveCaretLeft::operator=(EdiMoveCaretLeft&&) = default ;

    const std::string EdiMoveCaretLeft::sname() noexcept {
        return "edi_move_caret_left" ;
    }
    bool EdiMoveCaretLeft::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretLeft::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        move_caret_with_repeating(first_call, repeat_num, pimpl->ksr,
                [] {pushup(VKC_LSHIFT, VKC_LEFT) ;},
                [] {pushup(VKC_LEFT) ;}) ;
    }


    //EdiMoveCaretRight
    struct EdiMoveCaretRight::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretRight::EdiMoveCaretRight()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretRight::~EdiMoveCaretRight() noexcept                     = default ;
    EdiMoveCaretRight::EdiMoveCaretRight(EdiMoveCaretRight&&)            = default ;
    EdiMoveCaretRight& EdiMoveCaretRight::operator=(EdiMoveCaretRight&&) = default ;

    const std::string EdiMoveCaretRight::sname() noexcept {
        return "edi_move_caret_right" ;
    }
    bool EdiMoveCaretRight::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretRight::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        move_caret_with_repeating(first_call, repeat_num, pimpl->ksr,
                [] {pushup(VKC_LSHIFT, VKC_RIGHT) ;},
                [] {pushup(VKC_RIGHT) ;}) ;
    }


    //EdiMoveCaretUp
    struct EdiMoveCaretUp::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretUp::EdiMoveCaretUp()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretUp::~EdiMoveCaretUp() noexcept                  = default ;
    EdiMoveCaretUp::EdiMoveCaretUp(EdiMoveCaretUp&&)            = default ;
    EdiMoveCaretUp& EdiMoveCaretUp::operator=(EdiMoveCaretUp&&) = default ;

    const std::string EdiMoveCaretUp::sname() noexcept {
        return "edi_move_caret_up" ;
    }
    bool EdiMoveCaretUp::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretUp::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const parent_charlgr) const {
        auto v_press = [] {
            if(textselect::is_first_line_selection())
                textselect::select_line_EOL2BOL() ;

            keybrd::pushup(VKC_LSHIFT, VKC_UP) ;
            //textselect::moving_update() ;
        } ;
        auto n_press = [] {keybrd::pushup(VKC_UP) ;} ;

        if(parent_charlgr != nullptr) {
            auto str = parent_charlgr->to_str() ;
            if(str.empty()) return ;

            if(auto num = keyloggerutil::extract_num(str)) {
                move_caret_with_repeating(true, num, pimpl->ksr, v_press, n_press) ;
            }
        }
        else {
            move_caret_with_repeating(first_call, repeat_num, pimpl->ksr, v_press, n_press) ;
        }
    }

    //EdiMoveCaretDown
    struct EdiMoveCaretDown::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretDown::EdiMoveCaretDown()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretDown::~EdiMoveCaretDown() noexcept                    = default ;
    EdiMoveCaretDown::EdiMoveCaretDown(EdiMoveCaretDown&&)            = default ;
    EdiMoveCaretDown& EdiMoveCaretDown::operator=(EdiMoveCaretDown&&) = default ;

    const std::string EdiMoveCaretDown::sname() noexcept {
        return "edi_move_caret_down" ;
    }
    bool EdiMoveCaretDown::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretDown::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const parent_charlgr) const {
        auto v_press = [] {
            if(textselect::is_first_line_selection())
                textselect::select_line_BOL2EOL() ;

            keybrd::pushup(VKC_LSHIFT, VKC_DOWN) ;

            //If call EdiMoveCaretDown after EdiMoveCaretUp,
            //inner variables of moving_update() are dedicated to EOL2BOL.
            //so we cannot move caret down.
            //textselect::moving_update() ;
        } ;
        auto n_press = [] {keybrd::pushup(VKC_DOWN) ;} ;

        if(parent_charlgr != nullptr) {
            auto str = parent_charlgr->to_str() ;
            if(str.empty()) return ;

            if(auto num = keyloggerutil::extract_num(str)) {
                move_caret_with_repeating(true, num, pimpl->ksr, v_press, n_press) ;
            }
        }
        else {
            move_caret_with_repeating(first_call, repeat_num, pimpl->ksr, v_press, n_press) ;
        }
    }


    //EdiMoveCaretNwordsForward
    struct EdiNMoveCaretwordsForward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretwordsForward::EdiNMoveCaretwordsForward()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretwordsForward::~EdiNMoveCaretwordsForward() noexcept                             = default ;
    EdiNMoveCaretwordsForward::EdiNMoveCaretwordsForward(EdiNMoveCaretwordsForward&&)            = default ;
    EdiNMoveCaretwordsForward& EdiNMoveCaretwordsForward::operator=(EdiNMoveCaretwordsForward&&) = default ;

    const std::string EdiNMoveCaretwordsForward::sname() noexcept {
        return "edi_n_move_caret_words_forward" ;
    }
    bool EdiNMoveCaretwordsForward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretwordsForward::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        move_caret_with_repeating(first_call, repeat_num, pimpl->ksr,
            [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;},
            [] {pushup(VKC_LCTRL, VKC_RIGHT) ;}) ;
    }


    //EdiMoveCaretNwordsBackward
    struct EdiNMoveCaretwordsBackward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretwordsBackward::EdiNMoveCaretwordsBackward()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretwordsBackward::~EdiNMoveCaretwordsBackward() noexcept                              = default ;
    EdiNMoveCaretwordsBackward::EdiNMoveCaretwordsBackward(EdiNMoveCaretwordsBackward&&)            = default ;
    EdiNMoveCaretwordsBackward& EdiNMoveCaretwordsBackward::operator=(EdiNMoveCaretwordsBackward&&) = default ;

    const std::string EdiNMoveCaretwordsBackward::sname() noexcept {
        return "edi_n_move_caret_words_backward" ;
    }
    bool EdiNMoveCaretwordsBackward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretwordsBackward::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        move_caret_with_repeating(first_call, repeat_num, pimpl->ksr,
                [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;},
                [] {pushup(VKC_LCTRL, VKC_LEFT) ;}) ;
    }


    //EdiMoveCaretNWORDSForward
    struct EdiNMoveCaretWORDSForward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretWORDSForward::EdiNMoveCaretWORDSForward()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretWORDSForward::~EdiNMoveCaretWORDSForward() noexcept                             = default ;
    EdiNMoveCaretWORDSForward::EdiNMoveCaretWORDSForward(EdiNMoveCaretWORDSForward&&)            = default ;
    EdiNMoveCaretWORDSForward& EdiNMoveCaretWORDSForward::operator=(EdiNMoveCaretWORDSForward&&) = default ;

    const std::string EdiNMoveCaretWORDSForward::sname() noexcept {
        return "edi_n_move_caret_WORDS_forward" ;
    }
    bool EdiNMoveCaretWORDSForward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretWORDSForward::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        move_caret_with_repeating(first_call, repeat_num, pimpl->ksr,
                [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;},
                [] {pushup(VKC_LCTRL, VKC_RIGHT) ;}) ;
    }


    //EdiMoveCaretNWORDSBackward
    struct EdiNMoveCaretWORDSBackward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretWORDSBackward::EdiNMoveCaretWORDSBackward()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretWORDSBackward::~EdiNMoveCaretWORDSBackward() noexcept                              = default ;
    EdiNMoveCaretWORDSBackward::EdiNMoveCaretWORDSBackward(EdiNMoveCaretWORDSBackward&&)            = default ;
    EdiNMoveCaretWORDSBackward& EdiNMoveCaretWORDSBackward::operator=(EdiNMoveCaretWORDSBackward&&) = default ;

    const std::string EdiNMoveCaretWORDSBackward::sname() noexcept {
        return "edi_n_move_caret_WORDS_backward" ;
    }
    bool EdiNMoveCaretWORDSBackward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretWORDSBackward::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        move_caret_with_repeating(first_call, repeat_num, pimpl->ksr,
                [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;},
                [] {pushup(VKC_LCTRL, VKC_LEFT) ;}) ;
    }
}
