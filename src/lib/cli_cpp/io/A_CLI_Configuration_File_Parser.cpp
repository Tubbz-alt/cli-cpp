/**
 * @file    A_CLI_Configuration_File_Parser.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Configuration_File_Parser.hpp"

// CLI Libraries
#include "../cli/A_Connection_Manager_Socket_Config.hpp"
#include "../cmd/A_Command_Queue_Config.hpp"
#include "../cmd/A_Command_Parser_Factory.hpp"
#include "../thirdparty/pugixml.hpp"
#include "../utility/Log_Utilities.hpp"
#include "CLI_Configuration_File_Parser_Utilities.hpp"


// C++ Standard Libraries
#include <iostream>
#include <sstream>
#include <stdexcept>


// Boost Libraries
#include <boost/filesystem.hpp>


namespace CLI{
namespace IO{
namespace CONFIG{

/************************************/
/*          Constructor             */
/************************************/
A_CLI_Configuration_File_Parser::A_CLI_Configuration_File_Parser( const std::string& config_path )
  : m_class_name("A_CLI_Configuration_File_Parser"),
    m_config_pathname(config_path),
    m_current_configuration(A_CLI_Manager_Configuration(CORE::ConnectionType::SOCKET)),
    m_is_valid(false)
{
    // Parse the file
    Parse_Configuration_File();
}


/********************************************/
/*      Parse the Configuration File        */
/********************************************/
void A_CLI_Configuration_File_Parser::Parse_Configuration_File()
{

    /// List of queries
    const std::string CONNECTION_TYPE_QUERY   = "connection-type";
    const std::string COMMAND_CONFIG_NODE     = "command-configuration";
    const std::string CLI_CONFIG_QUERY        = "cli";
    const std::string CLI_TITLE_QUERY         = "title";
    const std::string CLI_COMMAND_QUEUE_QUERY = "command-queue"; 
    const std::string CLI_REDIRECT_QUERY      = "redirect";
    const std::string EVENT_MANAGER_QUERY     = "event-manager";


    // Temp Variables
    std::string temp_str;
    int temp_int;

    // Create XML Document
    pugi::xml_document xmldoc;
    pugi::xml_parse_result result = xmldoc.load_file( m_config_pathname.c_str() );

    if( result == false ){
        std::stringstream sin;
        sin << "File: " << __FILE__ << ", Line: " << __LINE__ << ". CLI Configuration File parsed with errors. Details: " << result.description();
        BOOST_LOG_TRIVIAL(error) << sin.str();
        return;
    }

    
    // Get the root node
    pugi::xml_node root_node = xmldoc.child("cli-manager-configuration");

    // Check the node
    if( root_node == pugi::xml_node() ){ return; }

    // Get the logging node
    bool        log_enabled;
    std::string log_path, log_severity;
    if( XML::Load_Logging_Config_XML_Node( root_node, false, log_enabled, log_path, log_severity ) != true )
    {
        // Error
        BOOST_LOG_TRIVIAL(error) << "File: " << __FILE__ << ", Line: " << __LINE__ << ". Unable to load the logging config node.";
    }


    if( log_enabled == true ){
        UTILS::Initialize_Logger( log_severity, log_path );
    }


    // Configure the connection information
    CORE::ConnectionType cli_conn_type;
    int window_rows, window_cols;
    if( XML::Load_Connection_Config_XML_Nodes( root_node, false,
                                               cli_conn_type,
                                               m_connection_manager_config,
                                               window_rows,
                                               window_cols ) != true )
    {
        BOOST_LOG_TRIVIAL(error) << "unable to load the Connection-Manager Configuration.";
        return;
    }

    // Check the connection type
    if( cli_conn_type == CORE::ConnectionType::UNKNOWN ){
        std::stringstream sin;
        sin << "error: " << __FILE__ << ", Line: " << __LINE__ << ". CLI Connection Type value is invalid.";
        std::cerr << sin.str() << std::endl;
        return;
    }
    m_current_configuration.Set_Connection_Type( cli_conn_type );
    
        
    // Set the connection manager config inside the CLI configuration
    if( m_connection_manager_config == nullptr ){
        BOOST_LOG_TRIVIAL(error) << "Unable to load the connection manager configuration. Currently null.";
        return;
    }
    m_current_configuration.Set_Connection_Manager_Config( m_connection_manager_config );
    
    // set the window size
    m_current_configuration.Set_Socket_Window_Rows( window_rows );
    m_current_configuration.Set_Socket_Window_Cols( window_cols );


    // Grab the CLI Node
    pugi::xml_node cli_node = root_node.child(CLI_CONFIG_QUERY.c_str());

    // Get the CLI title
    temp_str = cli_node.child(CLI_TITLE_QUERY.c_str()).attribute("value").as_string();
    if( temp_str == "" ){
        temp_str = "Console";
    }
    m_current_configuration.Set_CLI_Title( temp_str );

    
    // Get the CLI Command Queue Max Size
    temp_int = cli_node.child( CLI_COMMAND_QUEUE_QUERY.c_str()).attribute("max_size").as_int();
    if( temp_int <= 0 ){
        std::cerr << "CLI Command Queue Max Size must be > 0" << std::endl;
        return;
    }
    m_current_configuration.Set_Command_Queue_Config( CMD::A_Command_Queue_Config( temp_int ));
    
    
    // Get the redirect variables
    bool redirect_stdout = cli_node.child(CLI_REDIRECT_QUERY.c_str()).attribute("stdout").as_bool(false);
    bool redirect_stderr = cli_node.child(CLI_REDIRECT_QUERY.c_str()).attribute("stderr").as_bool(false);

    m_current_configuration.Set_Log_Window_Redirect_Flags( redirect_stdout,
                                                           redirect_stderr );

    // Grab the alias parameters
    pugi::xml_node alias_node = cli_node.child("alias-support");

    bool alias_support     = alias_node.attribute("enable").as_bool(false);
    std::string alias_path = alias_node.child("alias-list").attribute("pathname").as_string("");
   
    // Grab the Command Variable Parameters
    pugi::xml_node variable_node = cli_node.child("variable-support");

    bool variable_support     = variable_node.attribute("enable").as_bool(false);
    std::string variable_path = variable_node.child("variable-list").attribute("pathname").as_string("");

    // Parse the Async Message Timeout
    int async_refresh_time = cli_node.child("async-message-refresh-time").attribute("ms").as_int(-1);
    if( async_refresh_time < 0 )
    {
        async_refresh_time = 2000;
    }
    // Set config object
    m_current_configuration.Set_Async_Message_Refresh_Time(std::chrono::milliseconds(async_refresh_time));
    
    // Get the Command Parser config file
    temp_str = root_node.child(COMMAND_CONFIG_NODE.c_str()).attribute("path").as_string();
   

    // make sure it exists
    if( boost::filesystem::exists( temp_str ) == false ){
        std::cerr << "error:  " << temp_str << " does not exist." << std::endl;
        return;
    }

    // Parse the command file
    CMD::A_Command_Parser::ptr_t command_parser = CMD::A_Command_Parser_Factory::Initialize( temp_str, 
                                                                                             alias_support, alias_path,
                                                                                             variable_support, variable_path );

    // Make sure the parser is not null
    if( command_parser == nullptr ){
        std::cerr << "error: " << temp_str << " parsed with errors." << std::endl;
        return;
    }
    else{
        // Set the parser
        m_current_configuration.Set_Command_Parser(command_parser);
    }


    // Load the Event-Manager Configuration
    pugi::xml_node event_node = root_node.child(EVENT_MANAGER_QUERY.c_str());
    m_event_manager_config = XML::Load_Event_Manager_Config_XML_Node( event_node );
    
    // Pass the event manager configuration to the cli manager config
    m_current_configuration.Set_Event_Manager_Config( m_event_manager_config );

    
    // Set valid
    m_is_valid = true;
}


/**************************************************/
/*          Write the Configuration File          */
/**************************************************/
void A_CLI_Configuration_File_Parser::Write()
{
    // Create XML Document
    pugi::xml_document xmldoc;
    
    // Check if the configuration file exists
    if( boost::filesystem::exists( boost::filesystem::path(m_config_pathname)) == true )
    {
        // Load it if it does
        pugi::xml_parse_result result = xmldoc.load_file( m_config_pathname.c_str() );


        if( result == false ){
            std::stringstream sin;
            sin << "error: " << __FILE__ << ", Line: " << __LINE__ << ". CLI Configuration File parsed with errors. Details: " << result.description();
            std::cerr << sin.str() << std::endl;
            return;
        }

    }
    
    // Check the root node
    pugi::xml_node root_node = xmldoc.child("cli-manager-configuration");

    // make sure the root node exists
    if( root_node == pugi::xml_node() ){
        root_node = xmldoc.append_child(pugi::xml_node_type::node_element);
        root_node.set_name("cli-manager-configuration");
    }

    // Construct the Logging Node
    bool default_logging_enabled = true;
    std::string default_log_path = "log.log";
    std::string default_log_severity = "info";
    if( XML::Load_Logging_Config_XML_Node( root_node, true,
                                           default_logging_enabled,
                                           default_log_path,
                                           default_log_severity )          != true )
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to load the Logging XML Node.";
    }

        
   // Define our default connection manager configuration
   int def_portno = 12344;
   int def_sleep_time = 50000;
   int def_max_connections = 1;
   A_Connection_Manager_Base_Config::ptr_t connection_manager_config = std::make_shared<A_Connection_Manager_Socket_Config>( def_portno,
                                                                                                                             def_sleep_time,
                                                                                                                             def_max_connections );

    // Construct the Connection Type Node
    CORE::ConnectionType cli_conn_type = CORE::ConnectionType::SOCKET;
    int window_rows = 40, window_cols = 100;
    if( XML::Load_Connection_Config_XML_Nodes( root_node, true, cli_conn_type,
                                               connection_manager_config,
                                               window_rows,
                                               window_cols ) != true ){
        BOOST_LOG_TRIVIAL(error) << "Unable to load the Connection XML Nodes.";
    }
                                       

    // Save the file
    bool result = xmldoc.save_file( m_config_pathname.c_str() );
    if( result != true ){
        BOOST_LOG_TRIVIAL(error) << "Unable to save file.";
    }
}


/**********************************/
/*       Parse the CLI Node       */
/**********************************/
void A_CLI_Configuration_File_Parser::Parse_CLI_Node()
{



}


} // End of CONFIG Namespace
} // End of IO     Namespace
} // End of CLI    Namespace

