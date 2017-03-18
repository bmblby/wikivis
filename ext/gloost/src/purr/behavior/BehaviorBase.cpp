
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
#include <purr/behavior/BehaviorBase.h>
#include <purr/model/PurrModel.h>
#include <purr/action/ActionBase.h>
#include <purr/model/Node.h>


// cpp includes
#include <string>
#include <iostream>



namespace purr
{

namespace behavior
{


/**
  \class   BehaviorBase

  \brief   base class of all behaviors

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

BehaviorBase::BehaviorBase(const std::string& name):
  _name(name),
  _ownerNode(),
  _events(),
  _readWriteChangeMutex()
{
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
BehaviorBase::~BehaviorBase()
{
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
BehaviorBase::shared_ptr
BehaviorBase::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<BehaviorBase>(basePointer);
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the base mutex used to lock the behavior
  \remarks ...
*/

/*static*/
std::mutex&
BehaviorBase::getBaseMutex()
{
  return _readWriteChangeMutex;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the owner of the node
  \remarks ...
*/

/*virtual*/
std::shared_ptr<model::Node>
BehaviorBase::getNode()
{
  if (_ownerNode.expired())
  {
    return nullptr;
  }
  return _ownerNode.lock();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the owner of the node
  \remarks ...
*/

/*virtual*/
std::shared_ptr<const model::Node>
BehaviorBase::getNode() const
{
  if (_ownerNode.expired())
  {
    return nullptr;
  }
  return _ownerNode.lock();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the name of the behavior
  \remarks ...
*/

/*virtual*/
const std::string&
BehaviorBase::getName() const
{
  return _name;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called when the behavior is assigned to a Node
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::onAssignToNode(const std::shared_ptr<model::Node>& node)
{
  std::lock_guard<std::mutex> lock(getBaseMutex());
  _ownerNode = node;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::onBeginBehavior()
{
  std::cerr << std::endl << "BehaviorBase::onBeginBehavior(): " << this;
  std::cerr << std::endl;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is removed from model::Node
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::onEndBehavior()
{
  _ownerNode = std::weak_ptr<model::Node>();
  unmapAllEvents();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   maps an event::EventId to a std::function
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::mapEvent(const event::EventId& eventId,
                       std::function<void(std::shared_ptr<event::EventData>)> fun)
{
  _events[eventId] = fun;
}



////////////////////////////////////////////////////////////////////////////////


/**
  \brief   maps an event::EventId to a std::function
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::unmapEvent(const event::EventId& eventId)
{
  _events.erase(eventId);
}



////////////////////////////////////////////////////////////////////////////////


/**
  \brief   maps an event::EventId to a std::function
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::unmapAllEvents()
{
  _events.clear();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls a event
  \remarks ...
*/

/*virtual*/
void
BehaviorBase::callEvent(const event::EventId& eventId,
                        const std::shared_ptr<event::EventData>& data)
{
  auto iterator = _events.find(eventId);
  if (iterator != _events.end())
  {
    iterator->second(data);
  }
}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


