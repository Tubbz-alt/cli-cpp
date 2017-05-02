/**
 * @file    A_Connection_Manager_Event_Handler.cpp
 * @author  Marvin Smith
 * @date    6/1/2015
*/
#include "A_Connection_Manager_Event_Handler.hpp"

// CLI Libraries
#include "../utility/Log_Utilities.hpp"

// C++ Standard Libraries
#include <iostream>

namespace CLI{
namespace EVT{


/****************************/
/*       Constructor        */
/****************************/
A_Connection_Manager_Event_Handler::A_Connection_Manager_Event_Handler( A_Connection_Manager_Base::ptr_t connection_manager )
 : A_CLI_Event_Handler_Base(),
   m_class_name("A_Connection_Manager_Event_Handler"),
   m_connection_manager(connection_manager)
{
}


/****************************/
/*       Constructor        */
/****************************/
void A_Connection_Manager_Event_Handler::Process_Event( int const& instance, 
                                                        int const& event )
{
    // Check if we have a shutdown event
    if( event == (int)CLI_Event_Type::CLI_SHUTDOWN ){
        
        // Log
        CLI_LOG_CLASS( trace,
                       "CLI_SHUTDOWN Event triggered.  Disconnecting now.");
        
        // Set the connection flag
        m_connection_manager->Set_Is_Connected_Flag( instance, false );
    }

    // Check if we have a refresh event
    else if( event == (int)CLI_Event_Type::CLI_REFRESH )
    {

        // Log
        CLI_LOG_CLASS( trace,
                       "CLI_REFRESH Event triggered. Refreshing the screen now.");

        // Set the refresh command
        m_connection_manager->Refresh_Screen( instance );
        
        // Log
        CLI_LOG_CLASS( trace,
                       "CLI_REFRESH returned.");
    }
}


} // End of EVT Namespace
} // End of CLI Namespace

