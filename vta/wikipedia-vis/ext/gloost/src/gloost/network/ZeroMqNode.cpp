
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
#include <gloost/network/ZeroMqNode.h>
#include <gloost/BinaryBundle.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

/**
  \class   ZeroMqNode

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

ZeroMqNode::ZeroMqNode(unsigned contextId,
                       unsigned mode):
    _context(0),
    _contextId(contextId),
    _socket(0),
    _mode(mode),
    _running(false),
    _outQueueMutex(),
    _outQueue(),
    _inQueueMutex(),
    _inQueue()
{
  _context = new zmq::context_t(_contextId);
	_socket  = new zmq::socket_t((*_context), mode);
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

ZeroMqNode::~ZeroMqNode()
{
  delete _context;
  delete _socket;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   starts the thread
  \remarks ...
*/

/*virtual*/
void
ZeroMqNode::start()
{
  if (!_running)
  {
    _running = true;
    new boost::thread(boost::bind(&ZeroMqNode::run, this));
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
void
ZeroMqNode::stop()
{
  _running = false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns true if client is running
  \param   ...
  \remarks ...
*/

bool
ZeroMqNode::isRunning() const
{
	return _running;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   queues a BinaryBundle for sending
  \param   ...
  \remarks ...
*/

/*virtual */
void
ZeroMqNode::queue(BinaryBundle::shared_ptr bundle)
{
  boost::mutex::scoped_lock lock(_outQueueMutex);
  _outQueue.push_back(bundle);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a BinaryBundle from the recieved bundles and removes it from the buffer
  \param   ...
  \remarks ...
*/

/*virtual */
BinaryBundle::shared_ptr
ZeroMqNode::getRecievedBundle()
{
  if (_inQueue.size())
  {
    boost::mutex::scoped_lock lock(_inQueueMutex);
    BinaryBundle::shared_ptr bundle = _inQueue.front();
    _inQueue.pop_front();
    return bundle;
  }

  return 0;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the last BinaryBundle received  and removes it from the buffer
  \param   ...
  \remarks ...
*/

/*virtual */
BinaryBundle::shared_ptr
ZeroMqNode::getLastRecievedBundle()
{
  if (_inQueue.size())
  {
    boost::mutex::scoped_lock lock(_inQueueMutex);
    BinaryBundle::shared_ptr bundle = _inQueue.back();
    _inQueue.pop_back();
    return bundle;
  }

  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the number of BinaryBundles currently in the out queue
  \param   ...
  \remarks ...
*/

/*virtual */
unsigned
ZeroMqNode::getOutQueueSize()
{
  boost::mutex::scoped_lock lock(_outQueueMutex);
  return _outQueue.size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the number of BinaryBundles currently in the in queue
  \param   ...
  \remarks ...
*/

/*virtual */
unsigned
ZeroMqNode::getInQueueSize()
{
  boost::mutex::scoped_lock lock(_inQueueMutex);
  return _inQueue.size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   cleares the queue with the received Messages
  \param   ...
  \remarks ...
*/

/*virtual */
void
ZeroMqNode::clearReceivedMessges()
{
  boost::mutex::scoped_lock lock(_inQueueMutex);

  while (_inQueue.size())
  {
    _inQueue.pop_front();
  }
}


////////////////////////////////////////////////////////////////////////////////

/*virtual */
void
ZeroMqNode::sendMessage()
{
  boost::mutex::scoped_lock lock(_outQueueMutex);
  if (_outQueue.size())
  {
    BinaryBundle::shared_ptr bundle = _outQueue.front();

    zmq::message_t outMessage (bundle->getSize());
    memcpy ((void *) outMessage.data (), bundle->data(), bundle->getSize());
    _outQueue.pop_front();

//    _socket->send (outMessage);
  }
}

////////////////////////////////////////////////////////////////////////////////

/*virtual */
void
ZeroMqNode::receiveMessage()
{
  zmq::message_t inMessage;
  _socket->recv (&inMessage);

  if (inMessage.size())
  {
    BinaryBundle::shared_ptr bundle = BinaryBundle::create((unsigned char*)inMessage.data(),
                                                            inMessage.size());

    boost::mutex::scoped_lock lock(_inQueueMutex);
    _inQueue.push_back(bundle);
  }
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


