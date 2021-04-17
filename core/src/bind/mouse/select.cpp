#include "bind/mouse/select.hpp"

#include "bind/base/char_logger.hpp"
#include "bind/base/ntype_logger.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "util/def.hpp"

namespace vind
{
    //SelectAll
    SelectAll::SelectAll()
    : BindedFuncCreator("select_all")
    {}

    void SelectAll::sprocess() {
        mouse::click(KEYCODE_MOUSE_LEFT) ;
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_A) ;
    }
    void SelectAll::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectAll::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //ForwardUINavigation
    ForwardUINavigation::ForwardUINavigation()
    : BindedFuncCreator("forward_ui_navigation")
    {}
    void ForwardUINavigation::sprocess(unsigned int repeat_num) {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_TAB) ;
        }
    }
    void ForwardUINavigation::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void ForwardUINavigation::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //BackwardUINavigation
    BackwardUINavigation::BackwardUINavigation()
    : BindedFuncCreator("backward_ui_navigation")
    {}
    void BackwardUINavigation::sprocess(unsigned int repeat_num) {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_TAB) ;
        }
    }
    void BackwardUINavigation::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void BackwardUINavigation::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //DecideFocusedUIObject
    DecideFocusedUIObject::DecideFocusedUIObject()
    : BindedFuncCreator("decide_focused_ui_object")
    {}
    void DecideFocusedUIObject::sprocess() {
        keybrd::pushup(KEYCODE_SPACE) ;
    }
    void DecideFocusedUIObject::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void DecideFocusedUIObject::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
