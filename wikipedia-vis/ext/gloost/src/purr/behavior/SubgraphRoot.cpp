
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
#include <purr/behavior/SubgraphRoot.h>
#include <purr/model/SubgraphDecoratorBase.h>
#include <purr/action/ActionBase.h>
#include <purr/action/CallEvent.h>
#include <purr/model/Node.h>
#include <purr/model/PurrModel.h>
#include <purr/event/common.h>
#include <purr/event/EventData.h>

// gloost includes
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <iostream>



namespace purr
{

namespace behavior
{


/**
  \class   SubgraphRoot

  \brief   base class of all behaviors

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string SubgraphRoot::BehaviorName = std::string("purr::behavior::SubgraphRoot");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
SubgraphRoot::shared_ptr
SubgraphRoot::create(const std::shared_ptr<model::SubgraphDecoratorBase>& decorator)
{
	return shared_ptr(new SubgraphRoot(decorator));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

SubgraphRoot::SubgraphRoot(const std::shared_ptr<model::SubgraphDecoratorBase>& decorator):
  BehaviorBase(SubgraphRoot::BehaviorName),
  _decorator(decorator)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
SubgraphRoot::~SubgraphRoot()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
SubgraphRoot::shared_ptr
SubgraphRoot::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<SubgraphRoot>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
SubgraphRoot::onBeginBehavior()
{
//  mapEvent(purr::event::onMouseDrag,
//           std::bind(&SubgraphRoot::dragParent, this));
//
//  mapEvent(purr::event::onEndMouseDrag,
//           std::bind(&SubgraphRoot::dragParent, this));
//
//  mapEvent(purr::event::onEnterFrame,
//           std::bind(&SubgraphRoot::frameStep, this));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the subgraphDecorator
  \remarks ...
*/

/*virtual*/
std::shared_ptr<model::SubgraphDecoratorBase>
SubgraphRoot::getDecorator()
{
  return _decorator;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the subgraphDecorator
  \remarks ...
*/

/*virtual*/
std::shared_ptr<const model::SubgraphDecoratorBase>
SubgraphRoot::getDecorator() const
{
  return _decorator;
}

////////////////////////////////////////////////////////////////////////////////



} // namespace behavior
} // namespace purr


