
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


// gloost includes
#include <gloost/postal/QueueSubscriber.h>
#include <gloost/postal/Hub.h>
#include <gloost/postal/Message.h>


// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace postal
{


/**
  \class   QueueSubscriber

  \brief   A gloost::postal::Subscriber that stores incomming gloost::Postal::Message(s) in a queue

  \author  Felix Weiszig
  \date    December 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a QueueSubscriber instance
  \remarks ...
*/

/*static*/
QueueSubscriber::shared_ptr
QueueSubscriber::create(size_t queueCapacity)
{
	return shared_ptr(new QueueSubscriber(queueCapacity));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a QueueSubscriber instance
  \remarks ...
*/

/*static*/
QueueSubscriber::shared_ptr
QueueSubscriber::create(const std::string& name, size_t queueCapacity)
{
	return shared_ptr(new QueueSubscriber(name, queueCapacity));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

QueueSubscriber::QueueSubscriber(size_t queueCapacity):
  Subscriber("unamed QueueSubscriber"),
  _queueCapacity(queueCapacity),
  _messageQueue(),
  _mutex_messageQueue()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

QueueSubscriber::QueueSubscriber(const std::string& name, size_t queueCapacity):
  Subscriber(name),
  _queueCapacity(queueCapacity),
  _messageQueue(),
  _mutex_messageQueue()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
QueueSubscriber::~QueueSubscriber()
{
//	std::cerr << std::endl << "(QueueSubscriber::~QueueSubscriber()): " << this;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   receive a message
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
QueueSubscriber::receive(const std::string& path, Message::shared_ptr message)
{
  boost::mutex::scoped_lock lock(_mutex_messageQueue);

  // return false if queue was full
  if (_queueCapacity && _messageQueue.size() == _queueCapacity)
  {
    return false;
  }
  _messageQueue.push(message);

  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   pops and returns the next message from queue or returns a nullptr
  \param   ...
  \remarks ...
*/

/*virtual*/
Message::shared_ptr
QueueSubscriber::popMessage()
{
  boost::mutex::scoped_lock lock(_mutex_messageQueue);
  if (_messageQueue.size())
  {
    Message::shared_ptr message = _messageQueue.front();
    _messageQueue.pop();
    return message;
  }
  return nullptr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the last message from queue or returns a nullptr
  \param   ...
  \remarks ...
*/

/*virtual*/
Message::shared_ptr
QueueSubscriber::getLatestMessage()
{
  boost::mutex::scoped_lock lock(_mutex_messageQueue);
  if (!_messageQueue.size())
  {
    return nullptr;
  }
  return _messageQueue.back();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the max number of messages that fit in the queue
  \param   ...
  \remarks ...
*/

/*virtual*/
size_t
QueueSubscriber::getQueueCapacity() const
{
  return _queueCapacity;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the number of messages in the queue
  \param   ...
  \remarks ...
*/

/*virtual*/
size_t
QueueSubscriber::getQueueSize() const
{
  return _messageQueue.size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   clears the message queue
  \param   ...
  \remarks ...
*/

/*virtual*/
void
QueueSubscriber::clearQueue()
{
  _messageQueue = std::queue<Message::shared_ptr>();
}


////////////////////////////////////////////////////////////////////////////////





} // namespace postal
} // namespace gloost

