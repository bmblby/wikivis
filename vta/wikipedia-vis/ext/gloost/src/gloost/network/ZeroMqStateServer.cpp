
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
#include <gloost/network/ZeroMqStateServer.h>
#include <gloost/BinaryBundle.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   ZeroMqStateServer

  \brief   A server receiving custom data, sends all clients the current state data

  \author  Felix Weiszig
  \date    November 2011
  \remarks Usage Example: Client sends input -> Server aswers with current application state
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

ZeroMqStateServer::ZeroMqStateServer(unsigned contextId,
                                     unsigned mode):
    ZeroMqNode(contextId, mode),
    _currentStateDataMutex(),
    _currentStateData(0)
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

ZeroMqStateServer::~ZeroMqStateServer()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   connects the client to the server
  \param   ...
  \remarks ...
*/

void
ZeroMqStateServer::connect(const std::string& address, const std::string& port)
{
  std::string connectString = (address + ":" + port);

  std::cerr << std::endl << "Message from ZeroMqStateServer::connect(): ";
  std::cerr << std::endl << "             Binding " << connectString;

  _socket->bind (connectString.c_str());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   connects the client to the server
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ZeroMqStateServer::run()
{
  std::cerr << std::endl << "ZeroMqStateServer::run(): ";

  // server
  while (_running)
  {
    // this will stall until a message arrives
//    receiveMessage();
//    clearReceivedMessges();
    sendMessage();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   queues a BinaryBundle for sending
  \param   ...
  \remarks ...
*/

void
ZeroMqStateServer::queue(BinaryBundle* bundle)
{
  boost::mutex::scoped_lock lock(_currentStateDataMutex);

  if (_currentStateData)
  {
    delete _currentStateData;
  }

  _currentStateData = bundle;
}


////////////////////////////////////////////////////////////////////////////////

void
ZeroMqStateServer::sendMessage()
{
  if (_currentStateData)
  {
    boost::mutex::scoped_lock lock(_currentStateDataMutex);

    zmq::message_t outMessage (_currentStateData->getSize());
    memcpy ((void *) outMessage.data (), _currentStateData->data(), _currentStateData->getSize());

    _socket->send (outMessage);


    delete _currentStateData;
    _currentStateData = 0;
  }
//  else
//  {
//    // send empty signal
//   zmq::message_t request;
//  _socket->send (request);
//  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the number of BinaryBundles currently in the in queue
  \param   ...
  \remarks ZeroMqStateServer can have only one Bundle queued, which is the current state
*/

/*virtual */
unsigned
ZeroMqStateServer::getOutQueueSize()
{
  boost::mutex::scoped_lock lock(_currentStateDataMutex);
  return (bool)_currentStateData;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost



