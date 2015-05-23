/**
 * @file    NCurses_Utilities.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "NCurses_Utilities.hpp"

// C++ Standard Libraries
#include <iostream>

namespace CLI{
namespace NCURSES{


/**********************************/
/*          Constructor           */
/**********************************/
An_NCurses_Context::An_NCurses_Context()
  : screen(NULL),
    main_window(NULL),
    tty_terminal_name(""),
    tty_fd(NULL),
    tty_in(NULL),
    tty_out(NULL),
    keyboard_blocking(false),
    keyboard_timeout_usec(1000),
    m_class_name("An_NCurses_Context")
{
}


/*****************************/
/*        Destructor         */
/*****************************/
An_NCurses_Context::~An_NCurses_Context()
{
    // Delete
    if( tty_fd != nullptr &&
        tty_fd != NULL )
    {
        delete [] tty_fd;
        tty_fd = nullptr;
    }
}


/*************************************/
/*      Create NCurses Context       */
/*************************************/
void Initialize( An_NCurses_Context::ptr_t  context )
{
    // Make sure the context is not null
    if( context == nullptr ){
        std::cout << "warning: context is null" << std::endl;
        return;
    }
    
    // Initialize
    initscr();

    // Clear the window
    clear();
    noecho();

    // Set the keyboard
    keypad( stdscr, TRUE );

    // Prevent blocking
    nodelay( stdscr, !(context->keyboard_blocking) );
    
    // Initialize Color
    start_color();

    // Initialize Color Pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
}


/****************************************/
/*      Finalize NCurses Context        */
/****************************************/
void Finalize( An_NCurses_Context::ptr_t   context )
{

    // Make sure the context is not null
    if( context == nullptr ){
        return;
    }

    
    // End the window
    endwin();

}


/*****************************/
/*      Abort NCurses        */
/*****************************/
void Abort()
{
    // Shutdown
    endwin();
}



} // End of NCURSES Namespace
} // End of CLI     Namespace

