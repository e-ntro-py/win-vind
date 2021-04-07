#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    class CommandMode : public BindedFuncWithCreator<CommandMode> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit CommandMode() ;
        static const std::string sname() noexcept ;
        void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) const ;

        virtual ~CommandMode() noexcept ;
        CommandMode(CommandMode&&) ;
        CommandMode& operator=(CommandMode&&) ;
        CommandMode(const CommandMode&)            = delete ;
        CommandMode& operator=(const CommandMode&) = delete ;
    } ;
}

#endif