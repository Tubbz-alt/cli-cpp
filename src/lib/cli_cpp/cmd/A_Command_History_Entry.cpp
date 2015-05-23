/**
 * @file    A_Command_History_Entry.cpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#include "A_Command_History_Entry.hpp"

namespace CLI{
namespace CMD{

/**********************************/
/*          Constructor           */
/**********************************/
A_Command_History_Entry::A_Command_History_Entry( const int&                    command_id,
                                                  const std::string&            command_string,
                                                  const CMD::A_Command_Result&  command_result )
  : m_command_id(command_id),
    m_command_string(command_string),
    m_command_result(command_result)
{
}

} // End of CMD Namespace
} // End of CLI Namespace

