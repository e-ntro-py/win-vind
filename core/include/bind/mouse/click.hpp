#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct ClickLeft : public BindedFuncWithCreator<ClickLeft> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct ClickRight : public BindedFuncWithCreator<ClickRight> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif