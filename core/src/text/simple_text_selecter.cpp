#include "simple_text_selecter.hpp"

#include "io/keybrd_eventer.hpp"

namespace
{
    using namespace vind::textselect ;
    Mode g_mode{Mode::Unselect} ;

    bool g_first_line_selection = false ;
    bool g_first_call = true ;

    inline void _reset() noexcept {
        g_first_call = true ;
        g_first_line_selection = false ;
        g_mode = Mode::Unselect ;
    }
}

namespace vind
{
    namespace textselect
    {
        void select_words() noexcept {
            g_mode = Mode::Word ;
        }

        void select_line_EOL2BOL() {
            g_first_line_selection = g_first_call ;

            keybrd::pushup(VKC_END) ;
            keybrd::pushup(VKC_LSHIFT, VKC_HOME) ;

            g_mode = Mode::EOL2BOL ;
            g_first_call = false ;
        }

        void select_line_BOL2EOL() {
            g_first_line_selection = g_first_call ;

            keybrd::pushup(VKC_HOME) ;
            keybrd::pushup(VKC_LSHIFT, VKC_END) ;

            g_mode = Mode::BOL2EOL ;
            g_first_call = false ;
        }

        void unselect() {
            if(g_mode == Mode::EOL2BOL) {
                keybrd::pushup(VKC_LEFT) ;
            }
            else if(g_mode == Mode::BOL2EOL) {
                keybrd::pushup(VKC_RIGHT) ;
            }
            _reset() ;
        }

        Mode getg_mode() noexcept {
            return g_mode ;
        }

        void moving_update() {
            if(g_mode == Mode::BOL2EOL)
                keybrd::pushup(VKC_LSHIFT, VKC_END) ;
            else
                keybrd::pushup(VKC_LSHIFT, VKC_HOME) ;
        }

        bool is_first_line_selection() noexcept {
            return g_first_line_selection ;
        }
    }
}
