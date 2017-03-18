
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
#include <gloost/postal/MessageWrapper.h>
#include <gloost/postal/Message.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace postal
{


/**
  \class   MessageWrapper

  \brief   base class of all MessageWrappers, inheritor performing serialization and unserialization of Messages
           of a specific type

  \author  Felix Weiszig
  \date    August 2013
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

MessageWrapper::MessageWrapper(unsigned typeId):
    _typeId(typeId)
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

MessageWrapper::MessageWrapper(std::shared_ptr<gloost::postal::Message> message):
    _typeId(0),
    _message(message)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
MessageWrapper::~MessageWrapper()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the message
  \param   ...
  \remarks ...
*/

std::shared_ptr<gloost::postal::Message>
MessageWrapper::getMessage()
{
  if (!_message)
  {
    toMessage();
  }
  return _message;
}



////////////////////////////////////////////////////////////////////////////////





} // namespace postal
} // namespace gloost


