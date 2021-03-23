#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP
#include "common/binded_func_with_creator.hpp"

namespace vind
{
    struct Change2Normal : public BindedFuncWithCreator<Change2Normal> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;


    struct Change2Insert : public BindedFuncWithCreator<Change2Insert> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;


    struct Change2Visual : public BindedFuncWithCreator<Change2Visual> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct MyConfigWindowNormal : public BindedFuncWithCreator<MyConfigWindowNormal> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct MyConfigWindowInsert : public BindedFuncWithCreator<MyConfigWindowInsert> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
