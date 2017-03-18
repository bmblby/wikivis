
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


// purr includes
#include <purr/behavior/StringEventDataToLabel.h>
#include <purr/model/PurrModel.h>
#include <purr/action/ActionBase.h>
#include <purr/action/CallEvent.h>
#include <purr/model/Node.h>

// gloost includes
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <iostream>
#include <functional>



namespace purr
{


namespace behavior
{


/**
  \class   StringEventDataToLabel

  \brief   listens to user defined event ids and sets received GenericEventData<std::string> as node label

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    feb 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string StringEventDataToLabel::BehaviorName = std::string("purr::behavior::StringEventDataToLabel");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
StringEventDataToLabel::shared_ptr
StringEventDataToLabel::create(const StringEventDataToLabel::EventIdSet& _eventIdsToListenTo)
{
	return shared_ptr(new StringEventDataToLabel(_eventIdsToListenTo));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

StringEventDataToLabel::StringEventDataToLabel(const StringEventDataToLabel::EventIdSet& _eventIdsToListenTo):
  BehaviorBase(StringEventDataToLabel::BehaviorName),
  _eventIdsToListenTo(_eventIdsToListenTo)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
StringEventDataToLabel::~StringEventDataToLabel()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
StringEventDataToLabel::shared_ptr
StringEventDataToLabel::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<StringEventDataToLabel>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
StringEventDataToLabel::onBeginBehavior()
{
  for (const auto& eventId : _eventIdsToListenTo)
  {
    mapEvent(eventId,
             std::bind(&StringEventDataToLabel::setLabel,
                       this,
                       std::placeholders::_1));
  }

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the beginning of a step
  \remarks ...
*/

/*virtual*/
void
StringEventDataToLabel::setLabel(std::shared_ptr<purr::event::EventData> data)
{
  if (!data)
  {
    return;
  }

  if (auto stringData = event::GenericEventData<std::string>::castToThisType(data))
  {
    getNode()->setLabel(*stringData->getContent().get());

  }


}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


