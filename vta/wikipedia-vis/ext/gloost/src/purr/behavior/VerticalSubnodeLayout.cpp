
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
#include <purr/behavior/VerticalSubnodeLayout.h>
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
  \class   VerticalSubnodeLayout

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
VerticalSubnodeLayout::shared_ptr
VerticalSubnodeLayout::create()
{
	return shared_ptr(new VerticalSubnodeLayout());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

VerticalSubnodeLayout::VerticalSubnodeLayout():
  BehaviorBase("purr::behavior::VerticalSubnodeLayout"),
  _borderSize(0),
  _subnodeSpacing(0),
  _alignment(Alignment::CENTER)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
VerticalSubnodeLayout::~VerticalSubnodeLayout()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
VerticalSubnodeLayout::shared_ptr
VerticalSubnodeLayout::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<VerticalSubnodeLayout>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
VerticalSubnodeLayout::onBeginBehavior()
{
  mapEvent(purr::event::onChangeSize,
           std::bind(&VerticalSubnodeLayout::maintainLayout, this));

  mapEvent(purr::event::onAddSubnode,
           std::bind(&VerticalSubnodeLayout::maintainLayout, this));

  mapEvent(purr::event::onRemoveSubnode,
           std::bind(&VerticalSubnodeLayout::maintainLayout, this));

  mapEvent("maintainLayout",
           std::bind(&VerticalSubnodeLayout::maintainLayout, this));

//  mapEvent(purr::event::onSubnodeChangeSize,
//           std::bind(&VerticalSubnodeLayout::maintainLayout, this));

  maintainLayout();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the end of a step
  \remarks ...
*/

/*virtual*/
void
VerticalSubnodeLayout::maintainLayout()
{
  auto parent    = getNode();
  auto& subnodes = parent->getSubnodes();
  gloost::Point3 currentPos(_borderSize, parent->getSize()[1]-2*_borderSize, 0.0);

  unsigned index = 0u;

  auto heightSum = 2*_borderSize + _subnodeSpacing*(subnodes.size() - ((bool)subnodes.size()));

  for (const auto& subNode : subnodes)
  {
    if ((bool)index)
    {
      currentPos[1] -= _subnodeSpacing;
    }
    ++index;

    currentPos[1] -= subNode->getSize()[1];
    heightSum += subNode->getSize()[1];

    const auto offset = currentPos - subNode->getPosition();
    subNode->setPosition(subNode->getPosition() + offset);

    auto subNodeSize = subNode->getSize();
    subNodeSize[0] = parent->getSize()[0] - 2.0*_borderSize;
    const auto sizeSpeed = subNodeSize - subNode->getSize();
    subNode->setSize(subNode->getSize() + sizeSpeed);
  }

  auto size = parent->getSize();
  size[1] = heightSum;


  if (_alignment == Alignment::CENTER)
  {
    const auto parentCenterH = parent->getSize()[0]*0.5;
    for (const auto& subNode : subnodes)
    {
      const auto nodeCenterH = subNode->getPosition()[0] + subNode->getSize()[0]*0.5;
      subNode->setPosition({(gloost::mathType)(int)(parentCenterH-nodeCenterH),
                            (gloost::mathType)(int)(subNode->getPosition()[1]),
                            (gloost::mathType)(int)(subNode->getPosition()[2])});
    }
  }


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the size to the outer border
  \remarks ...
*/

/*virtual*/
void
VerticalSubnodeLayout::setBorderSize(float value)
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
VerticalSubnodeLayout::setSubnodeSpacing(float value)
{
  _subnodeSpacing = value;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the size to the outer border
  \remarks ...
*/

/*virtual*/
void
VerticalSubnodeLayout::setAlignmentMode(const VerticalSubnodeLayout::Alignment& value)
{
  _alignment = value;
}

////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


