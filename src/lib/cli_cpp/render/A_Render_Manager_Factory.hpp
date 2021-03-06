/**
 * @file    A_Render_Manager_Factory.hpp
 * @author  Marvin Smith
 * @date    5/22/2015
*/
#ifndef CLI_CPP__RENDER_A_RENDER_MANAGER_FACTORY_HPP
#define CLI_CPP__RENDER_A_RENDER_MANAGER_FACTORY_HPP

// CLI Libraries
#include "../core/ConnectionType.hpp"
#include "../core/SessionType.hpp"
#include "A_Render_Driver_Context_Base.hpp"
#include "A_Render_Manager_Base.hpp"

// C++ Standard Libraries
#include <memory>
#include <string>
#include <vector>

namespace CLI{
namespace RENDER{


/**
 * @class A_Render_Manager_Factory
*/
class A_Render_Manager_Factory
{
    public:


        /**
         * @brief Initialize the Render-Manager Factory.
         *
         * @param[in] conn_type Connection Type used for rendering.
         * @param[in] cli_title CLI Title to post to render-managers.
         * @param[in] command_parser Command-Parser to process command information.
        */
        static void Initialize( CMD::A_Command_Parser::ptr_t  command_parser,
                                CMD::A_Command_Queue::ptr_t   command_queue );


        /**
         * @brief Finalize the Render-Manager Factory instance.
         */
        static void Finalize();


        /**
         * @brief Get a Render-Manager Instance.
         *
         * @param[in] instance_id Instance of the render manager.
        */
        static A_Render_Manager_Base::ptr_t Instance_Of( int                instance_id,
                                                         CORE::SessionType  session_type );


        /**
         * @brief Register a Custom Render Window
         *
         * @param[in] render_window Render window to add to the system.
         */
        static int Register_Custom_Render_Window( An_ASCII_Render_Window_Base::ptr_t render_window );


        /**
         * @brief Check if Initialized.
         *
         * @return True if initialized, false otherwise.
        */
        static bool Is_Initialized();


        /**
         * @brief Send Asynchronous Message to Rendering Windows.
         *
         * @param[in] topic_name
         * @param[in] message
        */
        static void Send_Asynchronous_Message( const std::string& topic_name,
                                               const std::string& message );

    private:

        /**
         * @brief Default Constructor
        */
        A_Render_Manager_Factory();


        /**
         * @brief Get the global Factory Instance.
        */
        static A_Render_Manager_Factory& Get_Factory_Instance();


        /**
         * @brief Create a new instance.
         *
         * @param[in] instance_id ID to load into the render-manager.  Used for grabbing the driver and state.
         */
        A_Render_Manager_Base::ptr_t Create_Manager_Instance( int                instance_id,
                                                              CORE::SessionType  session_type )const;


        /// Class Name
        std::string m_class_name;

        /// Render Manager List
        std::vector<A_Render_Manager_Base::ptr_t> m_render_managers;

        /// Command Parser
        CMD::A_Command_Parser::ptr_t m_command_parser;

        /// Command Queue
        CMD::A_Command_Queue::ptr_t m_command_queue;

        /// Custom Window
        std::vector<An_ASCII_Render_Window_Base::ptr_t> m_custom_render_windows;

        /// Initialized Flag
        bool m_is_initialized;

}; // End of A_Render_Manager_Factory

} // End of RENDER Namespace
} // End of CLI    Namespace

#endif
