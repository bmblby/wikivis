
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
#include <gloost/postal/Message.h>


// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace postal
{


/**
  \class   Message

  \brief   ...

  \author  Felix Weiszig
  \date    December 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a Message instance
  \remarks ...
*/

/*static*/
Message::shared_ptr
Message::create(gloost::BinaryBundle::const_shared_ptr data)
{
	return shared_ptr(new Message(data));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Message::Message(gloost::BinaryBundle::const_shared_ptr data):
  _data(data)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Message::~Message()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the data of the message
  \remarks ...
*/

gloost::BinaryBundle::const_shared_ptr
Message::getBundle()
{
	return _data;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace postal
} // namespace gloost

