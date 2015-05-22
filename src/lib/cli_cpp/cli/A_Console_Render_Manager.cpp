/**
 * @file    A_Console_Render_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

// CLI Libraries
#include "../utils/ANSI_Utilities.hpp"
#include "../utils/Log_Utilities.hpp"

namespace CLI{


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager::A_Console_Render_Manager()
  : m_command_counter(0),
    m_waiting_command_response(false),
    m_waiting_command_response_value(nullptr),
    m_class_name("A_Console_Render_Manager"),
    m_command_history(std::make_shared<A_Command_History>())
{
}


/*************************************/
/*      Update the Command List      */
/*************************************/
void A_Console_Render_Manager::Update_Command_List( const std::vector<CMD::A_CLI_Command>& command_list )
{
    m_command_list = command_list;
}


/*************************************/
/*      Update the Command List      */
/*************************************/
void A_Console_Render_Manager::Update_Parser_Command_List( const std::vector<CMD::A_CLI_Parser_Command>& parser_command_list )
{
    m_parser_command_list = parser_command_list;
}


/****************************************************/
/*      Set the Command Waiting Response Values     */
/****************************************************/
void A_Console_Render_Manager::Set_Waiting_Command_Response( const CMD::A_CLI_Command_Result::ptr_t response ){
    m_waiting_command_response_value = response;
    m_waiting_command_response = true;
}


/**************************************************/
/*      Check the waiting response flag value     */
/**************************************************/
bool A_Console_Render_Manager::Check_Waiting_Command_Response(){
    
    // Log
    BOOST_LOG_TRIVIAL(trace) << "Start of " << __func__ << ". Status: " << std::boolalpha << m_waiting_command_response;
    
    // Avoid null
    if( m_waiting_command_response_value == nullptr ){ 
        return false; 
    }

    // Avoid if not waiting
    if( m_waiting_command_response == false ){
        return false;
    }

    // Check if still waiting
    if( m_waiting_command_response_value->Check_System_Response() == false &&
        m_waiting_command_response == true )
    {
        return true;
    }

    // Check if recieved
    if( m_waiting_command_response_value->Check_System_Response() &&
        m_waiting_command_response == true )
    {
        m_waiting_command_response = false;
        return true;
    }

    else{
        return false;
    }
}

} // End of CLI Namespace

