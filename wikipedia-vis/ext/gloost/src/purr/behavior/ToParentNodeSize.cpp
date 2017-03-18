
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


// purr includes
#include <purr/behavior/ToParentNodeSize.h>
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
  \class   ToParentNodeSize

  \brief

  \author  Felix Weiszig
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string ToParentNodeSize::BehaviorName = std::string("purr::behavior::ToParentNodeSize");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
ToParentNodeSize::shared_ptr
ToParentNodeSize::create(unsigned axis)
{
	return shared_ptr(new ToParentNodeSize(axis));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

ToParentNodeSize::ToParentNodeSize(unsigned axis):
  purr::behavior::BehaviorBase(ToParentNodeSize::BehaviorName),
  _axis(axis)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
ToParentNodeSize::~ToParentNodeSize()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
ToParentNodeSize::shared_ptr
ToParentNodeSize::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<ToParentNodeSize>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
ToParentNodeSize::onBeginBehavior()
{
  mapEvent(purr::event::onParentChangeSize,
           std::bind(&ToParentNodeSize::resizeByParentsSize, this));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief
  \remarks ...
*/

/*virtual*/
void
ToParentNodeSize::resizeByParentsSize()
{
  auto node = getNode();

  if (node && _axis < 3 && node->getParentNode())
  {
    auto size = node->getSize();
    size[_axis] = node->getParentNode()->getSize()[_axis];
    node->setSize(size);
  }
}

////////////////////////////////////////////////////////////////////////////////



} // namespace behavior

} // namespace purr


