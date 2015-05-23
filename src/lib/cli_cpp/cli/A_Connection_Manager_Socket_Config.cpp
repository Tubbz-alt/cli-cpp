/** 
 * @file    A_Connection_Manager_Socket_Config.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_Connection_Manager_Socket_Config.hpp"


namespace CLI{

/**************************/
/*      Constructor       */
/**************************/
A_Connection_Manager_Socket_Config::A_Connection_Manager_Socket_Config( const int& port )
  : A_Connection_Manager_Base_Config(),
    m_class_name("A_Connection_Manager_Socket_Config"),
    m_port(port)
{


}



/**************************/
/*      Destructor        */
/**************************/
A_Connection_Manager_Socket_Config::~A_Connection_Manager_Socket_Config()
{


}


} // End of CLI Namespace

