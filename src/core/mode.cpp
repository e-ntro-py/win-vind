#include "mode.hpp"

#include <array>
#include <mutex>
#include <stdexcept>
#include <unordered_map>


namespace
{
    using namespace vind::core ;

    class ModeManager {
        Mode mode_ ;
        ModeFlags flags_ ;
        mutable std::mutex mtx_ ;

        explicit ModeManager()
        : mode_(Mode::INSERT),
          flags_(ModeFlags::NONE),
          mtx_()
        {}

        virtual ~ModeManager() noexcept = default ;

    public:
        static ModeManager& get_instance() {
            static ModeManager instance ;
            return instance ;
        }

        void set_mode(Mode mode, ModeFlags flags) noexcept {
            std::lock_guard<std::mutex> scoped_lock{mtx_} ;

            mode_ = mode ;
            flags_ = flags ;
        }

        auto get_mode() const noexcept {
            std::lock_guard<std::mutex> scoped_lock{mtx_} ;
            return mode_ ;
        }

        auto get_flags() const noexcept {
            std::lock_guard<std::mutex> scoped_lock{mtx_} ;
            return flags_ ;
        }
    } ;
}


namespace vind
{
    namespace core
    {
        void set_global_mode(Mode mode, ModeFlags flags) noexcept {
            ModeManager::get_instance().set_mode(mode, flags) ;
        }

        Mode get_global_mode() noexcept {
            return ModeManager::get_instance().get_mode() ;
        }
        ModeFlags get_global_mode_flags() noexcept {
            return ModeManager::get_instance().get_flags() ;
        }

        std::string mode_to_prefix(Mode mode) noexcept {
            static auto to_prefix = [] {
                std::array<std::string, mode_num()> tmp ;
                tmp[static_cast<int>(Mode::INSERT)]     = "i" ;
                tmp[static_cast<int>(Mode::GUI_NORMAL)] = "gn" ;
                tmp[static_cast<int>(Mode::GUI_VISUAL)] = "gv" ;
                tmp[static_cast<int>(Mode::EDI_NORMAL)] = "en" ;
                tmp[static_cast<int>(Mode::EDI_VISUAL)] = "ev" ;
                tmp[static_cast<int>(Mode::COMMAND)]    = "c" ;
                tmp[static_cast<int>(Mode::RESIDENT)]   = "r" ;
                return tmp ;
            }() ;
            return to_prefix[static_cast<int>(mode)] ;
        }

        Mode parse_mode_prefix(const std::string& prefix) {
            static std::unordered_map<std::string, Mode> to_mode {
                {"i",  Mode::INSERT},
                {"gn", Mode::GUI_NORMAL},
                {"gv", Mode::GUI_VISUAL},
                {"en", Mode::EDI_NORMAL},
                {"ev", Mode::EDI_VISUAL},
                {"c",  Mode::COMMAND},
                {"r",  Mode::RESIDENT}
            } ;

            return to_mode.at(prefix) ;
        }

        const std::string& mode_to_name(Mode mode) noexcept {
            static auto to_str = [] {
                std::array<std::string, mode_num()> tmp ;
                tmp[static_cast<int>(Mode::INSERT)]     = "Insert" ;
                tmp[static_cast<int>(Mode::GUI_NORMAL)] = "GUI Normal" ;
                tmp[static_cast<int>(Mode::GUI_VISUAL)] = "GUI Visual" ;
                tmp[static_cast<int>(Mode::EDI_NORMAL)] = "Editor Normal" ;
                tmp[static_cast<int>(Mode::EDI_VISUAL)] = "Editor Visual" ;
                tmp[static_cast<int>(Mode::INSERT)]     = "Insert" ;
                tmp[static_cast<int>(Mode::COMMAND)]    = "Command" ;
                tmp[static_cast<int>(Mode::RESIDENT)]   = "Resident" ;
                return tmp ;
            }() ;
            return to_str[static_cast<int>(mode)] ;
        }
    }
}
