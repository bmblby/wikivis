
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
#include <purr/behavior/HorizontalSubnodeLayout.h>
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
  \class   HorizontalSubnodeLayout

  \brief   Behavior for a parent node. orders all subnodes below each other by respecting their
           height. subnodes are scaled to the width of the parent node

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
HorizontalSubnodeLayout::shared_ptr
HorizontalSubnodeLayout::create()
{
	return shared_ptr(new HorizontalSubnodeLayout());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

HorizontalSubnodeLayout::HorizontalSubnodeLayout():
  BehaviorBase("purr::behavior::HorizontalSubnodeLayout"),
  _borderSize(0),
  _subnodeSpacing(0)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
HorizontalSubnodeLayout::~HorizontalSubnodeLayout()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
HorizontalSubnodeLayout::shared_ptr
HorizontalSubnodeLayout::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<HorizontalSubnodeLayout>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
HorizontalSubnodeLayout::onBeginBehavior()
{
  mapEvent(purr::event::onChangeSize,
           std::bind(&HorizontalSubnodeLayout::maintainLayout, this));

  mapEvent(purr::event::onAddSubnode,
           std::bind(&HorizontalSubnodeLayout::maintainLayout, this));

  mapEvent(purr::event::onRemoveSubnode,
           std::bind(&HorizontalSubnodeLayout::maintainLayout, this));

  mapEvent("maintainLayout",
           std::bind(&HorizontalSubnodeLayout::maintainLayout, this));

//  mapEvent(purr::event::onSubnodeChangeSize,
//           std::bind(&HorizontalSubnodeLayout::maintainLayout, this));


  maintainLayout();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the end of a step
  \remarks ...
*/

/*virtual*/
void
HorizontalSubnodeLayout::maintainLayout()
{
  auto parentNode = getNode();
  auto& subnodes = parentNode->getSubnodes();
  gloost::Point3 currentPos(_borderSize, _borderSize, 0.0);
  unsigned index = 0u;
  auto widthSum = 2*_borderSize + _subnodeSpacing*(subnodes.size() - ((bool)subnodes.size()));

  for (const auto& subNode : subnodes)
  {
    if ((bool)index)
    {
      currentPos[0] += _subnodeSpacing;
    }
    ++index;

    const auto offset = currentPos - subNode->getPosition();
    subNode->setPosition(subNode->getPosition() + offset);

    currentPos[0] += subNode->getSize()[0];
    widthSum += subNode->getSize()[0];

    auto subNodeSize = subNode->getSize();
    subNodeSize[1] = parentNode->getSize()[1] - 2.0*_borderSize;
    const auto sizeSpeed = subNodeSize - subNode->getSize();
    subNode->setSize(subNode->getSize() + sizeSpeed);
  }

  auto size = getNode()->getSize();
  size[0] = widthSum;
  parentNode->setSize(size);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the size to the outer border
  \remarks ...
*/

/*virtual*/
void
HorizontalSubnodeLayout::setBorderSize(float value)
{
  _borderSize = value;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the size to the outer border
  \remarks ...
*/

/*virtual*/
void
HorizontalSubnodeLayout::setSubnodeSpacing(float value)
{
  _subnodeSpacing = value;
}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


