/**
 * @file    A_Command_Parser_Factory.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_Command_Parser_Factory.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>
#include <stdexcept>

// PUGI Xml
#include "../thirdparty/pugixml.hpp"

// CLI Libraries
#include "../utility/Log_Utilities.hpp"

namespace CLI{
namespace CMD{


/*******************************************************/
/*        Parse the Argument Autocomplete Node         */
/*******************************************************/
std::vector<std::string> Parse_Argument_Autocomplete( pugi::xml_node const& arg_auto_node )
{
    // check the node
    if( arg_auto_node == pugi::xml_node() ){
        return std::vector<std::string>();
    }

    // List of terms
    std::vector<std::string> output;

    // Get the internal nodes
    for( pugi::xml_node_iterator oit = arg_auto_node.begin(); oit != arg_auto_node.end(); oit++ )
    {
        // Check the name
        if( std::string(oit->name()) != "option" ){
            continue;
        }

        // Get the node
        pugi::xml_node option_node = (*oit);

        // Get the value
        std::string value = option_node.attribute("value").as_string();

        if( value.size() > 0 ){
            output.push_back(value);
        }

    }

    // return the list
    return output;

}


/********************************************************/
/*          Parse a Command Argument XML Node           */
/********************************************************/
A_Command_Argument Parse_Command_Argument_Node( pugi::xml_node const& arg_node )
{

    // Get the name
    std::string arg_name = arg_node.attribute("name").as_string();

    // Get the description
    std::string arg_description = arg_node.attribute("description").as_string();

    // Get the type
    CommandArgumentType arg_type = StringToCommandArgumentType(arg_node.attribute("type").as_string());

    // Get the required flag
    bool arg_required = arg_node.attribute("required").as_bool(true);

    // Get the default flag
    std::string arg_default_value = arg_node.attribute("default").as_string("");

    // Get any auto-complete terms
    std::vector<std::string> arg_auto_complete = Parse_Argument_Autocomplete( arg_node.child("auto-complete") );
    
    // Check the auto complete path
    bool arg_auto_complete_path = arg_node.attribute("auto-complete-path").as_bool(false);

    // return the autocomplete
    return A_Command_Argument( arg_name,
                               arg_type,
                               arg_description,
                               arg_required,
                               arg_default_value,
                               arg_auto_complete_path,
                               arg_auto_complete );

}

/*******************************************/
/*        Parse the CLI Commands           */
/*******************************************/
std::vector<A_CLI_Command> Parse_CLI_Commands( pugi::xml_node& cli_cmd_node )
{
    // Create list
    std::vector<A_CLI_Command> cli_list;
    bool mode_match;

    // Create list of tuple pairs
    std::vector<std::tuple<std::string,CMD::CommandParseStatus>> cli_command_reference = CMD::Get_CLI_Mode_To_Parse_Status_List();


    // Iterate over each command
    CMD::A_CLI_Command cli_command( CMD::CommandParseStatus::UNKNOWN );
    for( pugi::xml_node_iterator pit = cli_cmd_node.begin(); pit != cli_cmd_node.end(); pit++ )
    {

        // Get the node
        pugi::xml_node cli_node = (*pit);

        // Get the mode string
        std::string mode_str = cli_node.attribute("mode").as_string();

        
        // Iterate over each item in the reference, looking for matching mode
        mode_match = false;
        for( size_t i=0; i<cli_command_reference.size(); i++ ){
            
            // Check if the mode name strings match
            if( std::get<0>(cli_command_reference[i]) == mode_str ){
                
                // Create the node
                cli_command = CMD::A_CLI_Command( std::get<1>(cli_command_reference[i]));
                mode_match = true;
            }
        }

        // Fail if no match found
        if( mode_match == false ){    
            throw std::runtime_error("error: Unknown CLI command mode (" + mode_str + ")");
        }


        // Iterate over internal nodes 
        for( pugi::xml_node_iterator ait = cli_node.begin(); ait != cli_node.end(); ait++ )
        {
            
            // Add a name to the node
            if( std::string((*ait).name()) == "name" ){
                cli_command.Add_Name( (*ait).attribute("value").as_string());
            }

            // Process the description
            if( std::string((*ait).name()) == "description") {
                cli_command.Set_Description( (*ait).attribute("value").as_string());
            }

            // CLI_Task node
            if( std::string((*ait).name()) == "task-name" ){
                cli_command.Set_Formal_Name( (*ait).attribute("value").as_string());
            }

            // Check if arguments node
            if( std::string((*ait).name()) == "arguments" ){
                
                // Process each argument node
                for( pugi::xml_node_iterator arg_it = ait->begin(); arg_it != ait->end(); arg_it++ )
                {
            
                    // Check the name
                    if( std::string(arg_it->name()) == "argument" ){
                        cli_command.Add_Argument(Parse_Command_Argument_Node( *arg_it ));
                    }    
                }

            }

            // Response Expected
            if( std::string((*ait).name()) == "expect_response" ){
                cli_command.Set_Response_Expected( (*ait).attribute("value").as_bool(false));
            }
        }

        // Add CLI command
        cli_list.push_back(cli_command);

    }



    // Return list
    return cli_list;
}


/**************************************************/
/*          Parse the Standard Commands           */
/**************************************************/
std::vector<A_Command> Parse_Standard_Commands( pugi::xml_node& commands_node )
{
    // Output list
    std::vector<A_Command> command_list;

    
    // List of arguments
    std::vector<A_Command_Argument> argument_list;
    std::vector<std::string> arg_auto_complete;


    // Parse the Commands Node
    for( pugi::xml_node_iterator it = commands_node.begin(); it != commands_node.end(); it++ )
    {

        // Clear the argument list
        argument_list.clear();

        // Convert to node
        pugi::xml_node command_node = (*it);

        // Get the command name
        std::string command_name  = command_node.child("name").attribute("value").as_string();

        // Get the command description
        std::string command_description = command_node.child("description").attribute("value").as_string();

        // Check the required response
        bool command_response = command_node.child("expect_response").attribute("value").as_bool(false);

        // Get the arguments node
        pugi::xml_node arguments_node = command_node.child("arguments");
        for( pugi::xml_node_iterator ait = arguments_node.begin(); ait != arguments_node.end(); ait++ )
        {
            // Get the node
            pugi::xml_node arg_node = (*ait);
            
            // Check the name
            if( std::string(arg_node.name()) == "argument" ){
                argument_list.push_back( Parse_Command_Argument_Node( arg_node ));
            }    
        }

        // Add the command
        command_list.push_back(A_Command( command_name, command_description, command_response, argument_list ));

    }

    // return the command list
    return command_list;
}


/*********************************************/
/*          Create a Command Parser          */
/*********************************************/
A_Command_Parser::ptr_t  A_Command_Parser_Factory::Initialize( const std::string&  config_path,
                                                               bool                alias_support,
                                                               const std::string&  alias_path,
                                                               bool                variable_support,
                                                               const std::string&  variable_path )
{
    // Create XML Document
    pugi::xml_document xmldoc;
    pugi::xml_parse_result result = xmldoc.load_file( config_path.c_str() );

    /// Log init
    const std::string m_class_name = "A_Command_Parser_Factory";
    CLI_LOG_CLASS_ENTRY();

    if( result == false ){
        LOG_ERROR("CLI Command Configuration File parsed with errors. Details: " + std::string(result.description()));
        return nullptr;
    }

    // Lists
    std::vector<A_CLI_Command> cli_command_list;
    std::vector<A_Command> command_list;
    std::string regex_split_pattern;

    // Catch any exceptions
    try{

        // Get the root node
        pugi::xml_node root_node = xmldoc.child("command-configuration");

        // Check the node
        if( root_node == pugi::xml_node() ){ return nullptr; }


        // Get the regex split pattern
        regex_split_pattern = root_node.child("regex-split-pattern").attribute("value").as_string();


        // Get the Parser Command Nodes
        pugi::xml_node cli_commands_node = root_node.child("cli-commands");
        cli_command_list = Parse_CLI_Commands( cli_commands_node );


        // Get the Commands Node
        pugi::xml_node commands_node = root_node.child("commands");
        command_list = Parse_Standard_Commands( commands_node );

        // Command List
        std::vector<A_Command_Argument> argument_list;


        // Make sure the command lists have values
        if( cli_command_list.size() <= 0 ){
            throw std::runtime_error("CLI Command-List is currently empty. Configuration File Path (" + config_path + ").");
        }

        if( command_list.size() <= 0 ){
            throw std::runtime_error("Command-list is currently empty.");
        }


    } catch ( std::exception& e ) {
        BOOST_LOG_TRIVIAL(error) << "Exception caught in CLI Parser Factory. Details: " << e.what();
        return nullptr;
    }


    // If alias support is enabled, load the alias list path
    std::vector<A_Command_Alias> alias_list;
    if( alias_support == true ){
       alias_list = A_Command_Alias::Load_Alias_Configuration_File( alias_path );
    }
    

    // If variable support is enabled, load the variable list path
    std::vector<A_Command_Variable> variable_list;
    if( variable_support == true ){
        variable_list = A_Command_Variable::Load_Variable_Configuration_File( variable_path );
    }

    // Create the parser
    A_Command_Parser::ptr_t parser = std::make_shared<A_Command_Parser>( regex_split_pattern, 
                                                                         cli_command_list, 
                                                                         command_list,
                                                                         alias_list,
                                                                         alias_path,
                                                                         alias_support,
                                                                         variable_list,
                                                                         variable_path,
                                                                         variable_support );

    // Return new parser
    CLI_LOG_CLASS_EXIT();
    return parser;
}



} // End of CMD Namespace
} // End of CLI Namespace

