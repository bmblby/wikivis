
/*
                       ___                            __
                      /\_ \                          /\ \__
                   __ \//\ \     ___     ___     ____\ \  _\
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/
                   \_/__/

                   OpenGL framework for fast demo programming

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).
*/



// gloost system includes
#include <gloost/network/ZeroMqStateClient.h>
#include <gloost/BinaryBundle.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   ZeroMqStateClient

  \brief   A class template because I'm to lazy to write a class from scretch

  \author  Felix Weiszig
  \date    May 2011
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

ZeroMqStateClient::ZeroMqStateClient(unsigned contextId,
                                     unsigned mode):
    ZeroMqNode(contextId, mode)
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

ZeroMqStateClient::~ZeroMqStateClient()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   connects the client to the server
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ZeroMqStateClient::connect(const std::string& address, const std::string& port)
{
  std::string connectString = (address + ":" + port);

  std::cerr << std::endl << "Message from ZeroMqStateClient::connect(): ";
  std::cerr << std::endl << "             Connecting to " << connectString;

  _socket->connect (connectString.c_str());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   connects the client to the server
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ZeroMqStateClient::run()
{
  std::cerr << std::endl << "ZeroMqStateClient::run(): ";

  while (_running)
  {
    bool messageToDeliver = false;
    {
      boost::mutex::scoped_lock lock(_outQueueMutex);
      messageToDeliver = (bool)_outQueue.size();
    }

    if (messageToDeliver)
    {
      sendMessage();
      receiveMessage();
    }
  }

}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost



