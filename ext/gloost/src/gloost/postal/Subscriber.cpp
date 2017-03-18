
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
#include <gloost/postal/Subscriber.h>
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
  \class   Subscriber

  \brief   ...

  \author  Felix Weiszig
  \date    December 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a Subscriber instance
  \remarks ...
*/

/*static*/
Subscriber::shared_ptr
Subscriber::create()
{
	return shared_ptr(new Subscriber());
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Subscriber::Subscriber():
  _name("unamed Subscriber")
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a Subscriber instance
  \remarks ...
*/

/*static*/
Subscriber::shared_ptr
Subscriber::create(const std::string& name)
{
	return shared_ptr(new Subscriber(name));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Subscriber::Subscriber(const std::string& name):
  _name(name)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Subscriber::~Subscriber()
{
//	std::cerr << std::endl << "(Subscriber::~Subscriber()): " << this;
}


////////////////////////////////////////////////////////////////////////////////


#if 1

/**
  \brief   connects this Subscriber to a path
  \param   ...
  \remarks ...
*/

bool
Subscriber::connect(const std::string& path)
{
	return Hub::get()->connect(this->shared_from_this(), path);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   disconnect this node from a path
  \param   ...
  \remarks ...
*/

bool
Subscriber::disconnect(const std::string& path)
{
	return Hub::get()->disconnect(this->shared_from_this(), path);
}

#endif


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   receive a message
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
Subscriber::receive(const std::string& path, Message::shared_ptr message)
{
  std::cerr << std::endl;
	return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the name of the Subscriber
  \param   ...
  \remarks ...
*/

/*virtual*/
const std::string&
Subscriber::getName() const
{
	return _name;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace postal
} // namespace gloost

