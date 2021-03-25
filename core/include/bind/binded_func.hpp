#ifndef _BINDED_FUNC_HPP
#define _BINDED_FUNC_HPP

#include <memory>
#include <string>

#include "key_matcher.hpp"
#include "mode_manager.hpp"

#include "key/vkc_logger.hpp"
#include "key/char_logger.hpp"

namespace vind
{
    class BindedFunc {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        virtual void do_process(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* parent_vkclgr,
                const CharLogger* const parent_charlgr) const = 0 ;

    public:
        using shp_t = std::shared_ptr<BindedFunc> ;

        explicit BindedFunc() ;
        virtual ~BindedFunc() noexcept ;

        BindedFunc(BindedFunc&&) ;
        BindedFunc& operator=(BindedFunc&&) ;
        BindedFunc(const BindedFunc&)            = delete ;
        BindedFunc& operator=(const BindedFunc&) = delete ;

        virtual const std::string name() const noexcept = 0 ;

        void process(
                const bool first_call=true,
                const unsigned int repeat_num=1,
                VKCLogger* parent_vkclgr=nullptr,
                const CharLogger* const parent_charlgr=nullptr) const ;

        void register_matcher(
                const mode::Mode mode,
                const KeyMatcher::shp_t matcher) const ;
        void register_matcher(
                const unsigned char mode,
                const KeyMatcher::shp_t matcher) const ;

        //[return] matched num in latest log
        unsigned int validate_if_match(
                const KeyLoggerBase& pc_lgr,
                mode::Mode mode=mode::get_mode()) const ;

        //[return] matched num in latest log
        //The cost for computing is bigger than validate_if_match.
        unsigned int validate_if_fullmatch(
                const KeyLoggerBase& pc_lgr,
                mode::Mode mode=mode::get_mode()) const ;

        bool is_callable() const noexcept ;
        virtual bool is_for_moving_caret() const noexcept ;
    } ;
}
#endif
