#include "bindings_json_parser.hpp"

#include <iostream>
#include <algorithm>

#include "disable_gcc_warning.hpp"
#include "json.hpp" // libs/json.hpp (nlohmann)
#include "enable_gcc_warning.hpp"

#include "vkc_converter.hpp"
#include "msg_logger.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"

struct BindingsJSONParser::Impl
{
    const std::string filepath ;
    keymap_t bindings ;
    keymap_t commands ;
    vc_t ignored_syskeys ;

    explicit Impl(const std::string& path)
    : filepath(path),
      bindings(),
      commands(),
      ignored_syskeys(VKCConverter::get_all_sys_vkc())
    {
        bindings.clear() ;
        commands.clear() ;
        ignored_syskeys.clear() ;
    }

    virtual ~Impl() noexcept {
        bindings.clear() ;
        commands.clear() ;
        ignored_syskeys.clear() ;
    }
} ;

void BindingsJSONParser::init() noexcept
{
    auto catch_except = [](auto& e) {
        ERROR_STREAM << e.what() << " (BindingsJSONParser::init)\n" ;
    } ;
    try {
        std::ifstream ifs(pimpl->filepath) ; nlohmann::json jp{} ;
        ifs >> jp ;

        for(const auto& obj : jp) {
            try {
                const auto& name = obj["name"] ;
                const auto& keys = obj["key"] ;
                const auto& cmds = obj["cmd"] ;

                if(!keys.empty()) {
                    vvvc_t or_seqs{} ;
                    for(auto& key : keys) {
                        const auto seq = Utility::split(key, "->") ;

                        vvc_t seq_syncs{} ;
                        for(const auto& sync_str : seq) {
                            const auto sync = Utility::split(sync_str, "(+)") ;

                            vc_t sync_vkcs{} ;
                            for(const auto& keycode : sync) {
                                if(keycode.size() == 1) { //ascii code
                                    //ex) a
                                    if(auto vkc = VKCConverter::get_vkc(keycode[0])) {
                                        sync_vkcs.push_back(vkc) ;
                                        continue ;
                                    }
                                    //ex) A (A is divided to a and SHIFT)
                                    if(auto vkc = VKCConverter::get_shifted_vkc(keycode[0])) {
                                        sync_vkcs.push_back(vkc) ;
                                        sync_vkcs.push_back(VKC_SHIFT) ;
                                        continue ;
                                    }
                                }

                                //ex) Alt, Ctrl, ESC ...
                                const auto vkc = VKCConverter::get_sys_vkc(keycode) ;
                                sync_vkcs.push_back(vkc) ;
                                if(sync.size() == 1) {
                                    const auto pos = std::find(pimpl->ignored_syskeys.cbegin(), pimpl->ignored_syskeys.cend(), vkc) ;
                                    if(pos != pimpl->ignored_syskeys.cend()) {
                                        pimpl->ignored_syskeys.erase(pos) ;
                                    }
                                }
                            }
                            Utility::remove_deplication(sync_vkcs) ;
                            seq_syncs.push_back(std::move(sync_vkcs)) ;
                        }

                        or_seqs.push_back(std::move(seq_syncs)) ;
                    }
                    pimpl->bindings[name] = std::move(or_seqs) ;
                }

                if(!cmds.empty()) {
                    vvvc_t or_seqs{} ;
                    for(const std::string& cmdstr : cmds) {
                        vvc_t seq_syncs{} ;

                        //replace ascii to virtual keycode
                        for(const auto c : cmdstr) {
                            if(c == '*') {
                                seq_syncs.emplace_back(1, VKC_OPTIONAL) ;
                                continue ;
                            }
                            if(auto vkc = VKCConverter::get_vkc(c)) {
                                seq_syncs.emplace_back(1, vkc) ; //directly construct with std::vector{vkc}
                                continue ;
                            }
                            if(auto vkc = VKCConverter::get_shifted_vkc(c)) {
                                auto init_list = {VKC_SHIFT, vkc} ;
                                seq_syncs.emplace_back(std::move(init_list)) ;
                                continue ;
                            }
                        }

                        or_seqs.push_back(std::move(seq_syncs)) ;
                    }
                    pimpl->commands[name] = std::move(or_seqs) ;
                }
                }
            catch(nlohmann::json::exception& e) {
                catch_except(e) ;
                continue ;
            }
        }
    }
    catch(std::exception& e) {
        catch_except(e) ;
        return ;
    }
}

BindingsJSONParser::BindingsJSONParser(const std::string& filepath)
: pimpl(std::make_unique<Impl>(filepath))
{
    init() ;
}

BindingsJSONParser::~BindingsJSONParser() noexcept = default ;
BindingsJSONParser::BindingsJSONParser(BindingsJSONParser&&) noexcept = default ;
BindingsJSONParser& BindingsJSONParser::operator=(BindingsJSONParser&&) noexcept = default ;

const BindingsJSONParser::keymap_t& BindingsJSONParser::get_bindings() const noexcept
{
    return pimpl->bindings ;
}

const BindingsJSONParser::keymap_t& BindingsJSONParser::get_commands() const noexcept
{
    return pimpl->commands ;
}

const BindingsJSONParser::vc_t& BindingsJSONParser::get_ignored_syskeys() const noexcept
{
    return pimpl->ignored_syskeys ;
}