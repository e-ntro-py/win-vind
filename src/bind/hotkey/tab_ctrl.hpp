#ifndef _TAB_CTRL_HPP
#define _TAB_CTRL_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct Switch2LeftTab : public BindedFuncCreator<Switch2LeftTab> {
            explicit Switch2LeftTab() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct Switch2RightTab : public BindedFuncCreator<Switch2RightTab> {
            explicit Switch2RightTab() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct OpenNewTab : public BindedFuncCreator<OpenNewTab> {
            explicit OpenNewTab() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct CloseCurrentTab : public BindedFuncCreator<CloseCurrentTab> {
            explicit CloseCurrentTab() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif