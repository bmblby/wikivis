
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

             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/

// purr includes
#include <purr/action/ActionBase.h>
#include <purr/model/Node.h>

// cpp includes
#include <string>
#include <iostream>

namespace purr
{

namespace action
{

/**
  \class   ActionBase

  \brief   base class of all action/traverser

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief
  \remarks ...
*/

/*static*/
//ActionBase::shared_ptr
//ActionBase::create()
//{
//	shared_ptr model(new ActionBase("ActionBase"));
//	return model;
//}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

ActionBase::ActionBase(const std::string& type):
  _traversOrders(),
  _type(type),
  _travMask(GLOOST_BITMASK_ALL_SET)
{


}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
ActionBase::~ActionBase()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the type of the action
  \remarks ...
*/

/*virtual*/
const std::string&
ActionBase::getType() const
{
 return _type;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   begins the traversal on a specific node
  \remarks ...
*/

/*virtual*/
void
ActionBase::begin(const std::shared_ptr<model::Node>& startNode)
{
  std::cerr << std::endl << "From ActionBase::begin(): " << this;
  std::cerr << std::endl << "     beginning with node: " << startNode->getLabel();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the travMask
  \remarks ...
*/

/*virtual*/
const gloost::BitMask&
ActionBase::getTravMask() const
{
  return _travMask;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the travMask
  \remarks ...
*/

/*virtual*/
void
ActionBase::setTravMask(const gloost::BitMask& travMask)
{
  _travMask = travMask;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace model

} // namespace purr


