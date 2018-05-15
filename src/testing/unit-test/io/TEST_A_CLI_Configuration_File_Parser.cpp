/**
 * @file    TEST_A_CLI_Configuration_File_Parser.cpp
 * @author  Marvin Smith
 * @date    5/10/2018
 */
#include <gtest/gtest.h>

// CLI Libraries
#include <cli_cpp/io/A_CLI_Configuration_File_Parser.hpp>

// C++ Libraries


/**************************************************/
/*          Test the Parser (Valid File)          */
/**************************************************/
TEST( A_CLI_Configuration_File_Parser, Constructor_Valid_Path )
{
    // Config Pathname
    const std::string config_path = "data/CLI_Manager_Configuration.xml";
    
    // Create a Config-Parser
    auto parser = CLI::IO::CONFIG::A_CLI_Configuration_File_Parser( config_path );
    
    ASSERT_TRUE( parser.Is_Valid() );
    
    auto cli_config = parser.Get_CLI_Manager_Configuration();
    
}