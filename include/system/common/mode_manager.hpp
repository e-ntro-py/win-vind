#ifndef _MODE_MANAGER_HPP
#define _MODE_MANAGER_HPP

namespace ModeManager
{
    enum class Mode : unsigned char {
        Normal,
        Insert,
        Visual,
        Command,

        _EditorModeThreshold,
        EdiNormal,
        EdiInsert,
        EdiVisual,
        EdiLineVisual,
        EdiCommand,

        NUM,
    } ;

    void change_mode(const Mode mode) noexcept ;
    void change_mode(const int mode) noexcept ;
    void change_mode(const unsigned char mode) noexcept ;
    Mode get_mode() noexcept ;

    bool is_edi_visual() noexcept ;
    bool is_editor() noexcept ;

    bool is_command() noexcept ;
}

#endif