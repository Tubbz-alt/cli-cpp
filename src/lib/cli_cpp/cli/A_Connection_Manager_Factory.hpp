/**
 * @file    A_Connection_Manager_Factory.hpp
 * @author  Marvin Smith
 * @date    5/22/2015
*/
#ifndef __CLI_CMD_CLI_A_CONNECTION_MANAGER_FACTORY_HPP__
#define __CLI_CMD_CLI_A_CONNECTION_MANAGER_FACTORY_HPP__

// CLI Libraries
#include "A_Connection_Manager_Base.hpp"
#include "A_Connection_Manager_Base_Config.hpp"

namespace CLI{

/** 
 * @class A_Connection_Manager_Factory
*/
class A_Connection_Manager_Factory
{

    public:
        
        /**
         * @brief Initialize the connection manager.
         *
         * @return Connection manager.  Null if there was a problem.
        */
        static A_Connection_Manager_Base::ptr_t Initialize( A_Connection_Manager_Base_Config::ptr_t configuration );


}; // End of A_Connection_Manager_Factory Class

} // End of CLI Namespace

#endif
