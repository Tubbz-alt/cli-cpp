/**
 * @file    A_Socket_Telnet_Instance.hpp
 * @author  Marvin Smith
 * @date    7/7/2015
*/
#ifndef CLI_CPP_LIB_CLI_A_SOCKET_CONNECTION_INSTANCE_HPP
#define CLI_CPP_LIB_CLI_A_SOCKET_CONNECTION_INSTANCE_HPP

// C++ Standard Libraries
#include <memory>
#include <mutex>
#include <string>

// CLI Libraries
#include "../core/A_Session.hpp"
#include "../render/A_Render_Manager_Base.hpp"
#include "A_Socket_Base_Instance.hpp"

namespace CLI{


class A_Socket_Instance_Config_Telnet : public A_Socket_Instance_Config_Base
{
    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_Socket_Instance_Config_Telnet> ptr_t;
        
        /**
         * @brief Constructor
         */
        A_Socket_Instance_Config_Telnet( std::chrono::microseconds  read_sleep_timeout );
        
        
        /**
         * @brief Get the session-type
         */
        CORE::SessionType Get_Session_Type()const override{
            return CORE::SessionType::TELNET;
        }
        
        
        /**
         * @brief Print to Log String
         */
        virtual std::string To_Log_String( int indent )const;
        
        
    private:
        
        /// Class-Name
        std::string m_class_name;
        
};

/**
 * @class A_Socket_Telnet_Instance
*/
class A_Socket_Telnet_Instance : public A_Socket_Base_Instance
{

    public:

        /// Pointer Type
        typedef std::shared_ptr<A_Socket_Telnet_Instance> ptr_t;


        /**
         * @brief Constructor
         * 
         * @param[in] instance Instance ID of the client.
         * @param[in] client_fd Socket File Descriptor of Client.
         */
        A_Socket_Telnet_Instance( A_Socket_Instance_Config_Base::ptr_t  config,
                                  int                                   instance_id,
                                  const CORE::Session&                  session,
                                  int                                   client_fd );
        

        /**
         * @brief Destructor
        */
        virtual ~A_Socket_Telnet_Instance();

        
        /**
         * @brief Run
        */
        void Run() override;
        

        /**
         * @brief Refresh the Screen
        */
        void Refresh_Screen() override;


    private:
    
        /**
         * @brief Check for special keys.
         *
         * @param[in]  key_str String of digits from system.
         *
         * @return Key value.  -1 if no key present.
        */
        int Process_Special_Key( std::string const& key_str ) const;
       
        
        /**
         * @brief Configure the Special Key List.
        */
        void Configure_Special_Key_List();


        /// Class Name
        std::string m_class_name;
        
        /// Configuration
        A_Socket_Instance_Config_Telnet::ptr_t m_config;
    
        /// Local Render Manager Instance
        RENDER::A_Render_Manager_Base::ptr_t m_render_manager; 

        /// Keyboard Special Key Map
        std::vector<std::tuple<std::string,int>> m_special_key_list;
    
        /// Refresh Screen Mutex Lock
        std::mutex m_refresh_lock;

        /// Skip the Rendering
        bool m_skip_render;

        bool m_first_command_received;

        std::chrono::steady_clock::time_point m_last_refresh;

}; // End of A_Socket_Connection_Instance_Class


} // End of CLI Namespace

#endif

