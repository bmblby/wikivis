
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
#include <purr/behavior/DrawableEntity.h>
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
  \class   DrawableEntity

  \brief   store the type of a drawable node or subtree

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    feb. 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string DrawableEntity::BehaviorName = std::string("purr::behavior::DrawableEntity");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
DrawableEntity::shared_ptr
DrawableEntity::create(const DrawableEntity::EntityType& entityType)
{
	return shared_ptr(new DrawableEntity(entityType));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

DrawableEntity::DrawableEntity(const DrawableEntity::EntityType& entityType):
  BehaviorBase(DrawableEntity::BehaviorName),
  _entityType(entityType),
  _labelOffset(2,2,0)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
DrawableEntity::~DrawableEntity()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
DrawableEntity::shared_ptr
DrawableEntity::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<DrawableEntity>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
DrawableEntity::onBeginBehavior()
{
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   prepare the entity for drawing
  \remarks ...
*/

/*virtual*/
void
DrawableEntity::prepareToDraw()
{
//  std::lock_guard<std::mutex> lock(getBaseMutex());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the type of the drawable entity
  \remarks ...
*/

/*virtual*/
const DrawableEntity::EntityType&
DrawableEntity::getEntityType() const
{
  return _entityType;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the type of the drawable entity
  \remarks ...
*/

/*virtual*/
void
DrawableEntity::setLabelOffset(const gloost::Vector3& labelOffset)
{
  _labelOffset = labelOffset;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the type of the drawable entity
  \remarks ...
*/

/*virtual*/
const gloost::Vector3&
DrawableEntity::getLabelOffset() const
{
  return _labelOffset;
}


////////////////////////////////////////////////////////////////////////////////



} // namespace behavior

} // namespace purr


