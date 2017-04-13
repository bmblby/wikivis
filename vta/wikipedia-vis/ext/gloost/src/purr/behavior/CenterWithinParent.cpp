
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
#include <purr/behavior/CenterWithinParent.h>
#include <purr/model/PurrModel.h>
#include <purr/action/ActionBase.h>
#include <purr/model/Node.h>
#include <purr/event/common.h>

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
  \class   CenterWithinParent

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
CenterWithinParent::shared_ptr
CenterWithinParent::create(float hardness, float floating)
{
	return shared_ptr(new CenterWithinParent(hardness, floating));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

CenterWithinParent::CenterWithinParent(float hardness, float floating):
  BehaviorBase("purr::behavior::CenterWithinParent"),
  _speed(0.0,0.0,0.0),
  _hardness(hardness),
  _floating(floating)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
CenterWithinParent::~CenterWithinParent()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
CenterWithinParent::shared_ptr
CenterWithinParent::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<CenterWithinParent>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
CenterWithinParent::onBeginBehavior()
{
  mapEvent(purr::event::onEnterFrame,
           std::bind(&CenterWithinParent::enterFrame, this));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the beginning of a step
  \remarks ...
*/

/*virtual*/
void
CenterWithinParent::enterFrame()
{
  moveToParentsCenter();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the end of a step
  \remarks ...
*/

/*virtual*/
void
CenterWithinParent::moveToParentsCenter()
{
  auto parentNode = getNode()->getParentNode();

  if (parentNode == nullptr)
  {
    std::cerr << std::endl << " NO PARENT: " << this;
    return;
  }

  auto node = getNode();

  // in parent coords
  auto parentCenter = gloost::Point3(parentNode->getAabb().getHalfSize());
  auto nodeCenter   = node->getPosition() + node->getAabb().getHalfSize();

  _speed += (parentCenter-nodeCenter)*_hardness;
  _speed[2] = 0.0;

  if (_speed.length2() > GLOOST_MATHTYPE_MIN_EPSILON)
  {
    node->setPosition(node->getPosition()+_speed);
  }

  _speed *= _floating;
}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


