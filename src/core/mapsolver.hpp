#ifndef _MAPSOLVER_HPP
#define _MAPSOLVER_HPP

#include <memory>
#include <string>
#include <vector>

#include "cmdunit.hpp"
#include "mode.hpp"


namespace vind
{
    namespace core
    {
        class MapSolver {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit MapSolver(bool enable_typing_emulator=true) ;
            virtual ~MapSolver() noexcept ;

            MapSolver(const MapSolver&) = delete ;
            MapSolver& operator=(const MapSolver&) = delete ;

            MapSolver(MapSolver&&) ;
            MapSolver& operator=(MapSolver&&) ;

            void add_default(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            void add_map(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            void add_noremap(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            bool remove(const std::string& trigger_strcmd) ;
            bool remove_default(const std::string& trigger_strcmd) ;

            void deploy() ;
            void deploy_default(bool solve=true) ;

            void clear() ;
            void clear_default() ;

            void backward_state(std::size_t n) ;

            void reset_state() ;

            // If the trigger command is matched, it return true.
            bool map_command_to(
                const CmdUnit& raw_cmdunit,
                std::vector<CmdUnit::SPtr>& target_cmd,
                unsigned int& count) ;

            std::vector<std::vector<CmdUnit::SPtr>> get_trigger_commands() const ;
            std::vector<std::vector<CmdUnit::SPtr>> get_target_commands() const ;
        } ;
    }
}

#endif
