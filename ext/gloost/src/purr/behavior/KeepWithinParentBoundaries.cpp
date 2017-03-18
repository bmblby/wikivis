
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
#include <purr/behavior/KeepWithinParentBoundaries.h>
#include <purr/model/PurrModel.h>
#include <purr/action/ActionBase.h>
#include <purr/event/common.h>
#include <purr/model/Node.h>

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
  \class   KeepWithinParentBoundaries

  \brief   base class of all behaviors

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
KeepWithinParentBoundaries::shared_ptr
KeepWithinParentBoundaries::create()
{
	return shared_ptr(new KeepWithinParentBoundaries());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

KeepWithinParentBoundaries::KeepWithinParentBoundaries():
  BehaviorBase("purr::behavior::KeepWithinParentBoundaries")
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
KeepWithinParentBoundaries::~KeepWithinParentBoundaries()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
KeepWithinParentBoundaries::shared_ptr
KeepWithinParentBoundaries::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<KeepWithinParentBoundaries>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
KeepWithinParentBoundaries::onBeginBehavior()
{
  mapEvent(purr::event::onExitFrame,
           std::bind(&KeepWithinParentBoundaries::checkBoundariesAndSetPosition, this));
  checkBoundariesAndSetPosition();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the end of a step
  \remarks ...
*/

/*virtual*/
void
KeepWithinParentBoundaries::checkBoundariesAndSetPosition()
{
  std::lock_guard<std::mutex> lock(getBaseMutex());

  auto parentNode = getNode()->getParentNode();

  if (parentNode == nullptr)
  {
    return;
  }

  auto node = getNode();

  const auto& parentSize = parentNode->getSize();
  const auto& mySize = node->getSize();
  auto myPosition = node->getPosition();

  for (unsigned i=0u; i!=2; ++i)
  {
    if (myPosition[i] < 0.0)
    {
      myPosition[i] = 0.0;
      node->setPosition(myPosition);
    }
    else if (myPosition[i] + mySize[i] > parentSize[i])
    {
      myPosition[i] = parentSize[i] - mySize[i];
      node->setPosition(myPosition);
    }
  }

}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


