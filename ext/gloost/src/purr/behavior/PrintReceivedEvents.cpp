
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
#include <purr/behavior/PrintReceivedEvents.h>
#include <purr/model/PurrModel.h>
#include <purr/action/ActionBase.h>
#include <purr/action/CallEvent.h>
#include <purr/model/Node.h>
#include <purr/event/common.h>

// gloost includes
#include <gloost/gloostMath.h>
#include <gloost/postal/common.h>


// cpp includes
#include <string>
#include <atomic>
#include <iostream>



namespace purr
{


namespace behavior
{


/**
  \class   PrintReceivedEvents

  \brief   base class of all behaviors

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string PrintReceivedEvents::BehaviorName = std::string("purr::behavior::PrintReceivedEvents");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
PrintReceivedEvents::shared_ptr
PrintReceivedEvents::create()
{
	return shared_ptr(new PrintReceivedEvents());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

PrintReceivedEvents::PrintReceivedEvents():
  BehaviorBase("purr::behavior::PrintReceivedEvents"),
  _eventCounter(0u),
  _printEventMutex()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
PrintReceivedEvents::~PrintReceivedEvents()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
PrintReceivedEvents::shared_ptr
PrintReceivedEvents::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<PrintReceivedEvents>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
PrintReceivedEvents::onBeginBehavior()
{
  mapEvent(purr::event::onMouseDown,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseDown));

  mapEvent(purr::event::onMouseUp,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseUp));

  mapEvent(purr::event::onMouseUpOutside,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseUpOutside));

  mapEvent(purr::event::onMouseEnter,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseEnter));

  mapEvent(purr::event::onMouseLeave,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseLeave));

  mapEvent(purr::event::onBeginMouseDrag,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onBeginMouseDrag));

  mapEvent(purr::event::onMouseDrag,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseDrag));

  mapEvent(purr::event::onEndMouseDrag,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onEndMouseDrag));

  mapEvent(purr::event::onMouseDrop,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseDrop));

  mapEvent(purr::event::onBeginMouseHover,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onBeginMouseHover));

  mapEvent(purr::event::onMouseHover,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onMouseHover));

  mapEvent(purr::event::onEndMouseHover,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onEndMouseHover));


  mapEvent(purr::event::onKeyDown,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onKeyDown));

  mapEvent(purr::event::onKeyUp,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onKeyUp));

  mapEvent(purr::event::onAddSubnode,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onAddSubnode));


  mapEvent(purr::event::onRemoveSubnode,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onRemoveSubnode));

  mapEvent(purr::event::onSetParentNode,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onSetParentNode));

  mapEvent(purr::event::onAddSibling,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onAddSibling));

  mapEvent(purr::event::onRemoveSibling,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onRemoveSibling));

  mapEvent(purr::event::onChangePosition,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onChangePosition));

  mapEvent(purr::event::onChangeSize,
           std::bind(&PrintReceivedEvents::printReceivedEvent, this, purr::event::onChangeSize));
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the beginning of a step
  \remarks ...
*/

/*virtual*/
void
PrintReceivedEvents::printReceivedEvent(const std::string& eventName)
{
  std::lock_guard<std::mutex> lock(_printEventMutex);
  {
    gloost::postal::common::ColoredStringMessage::send(gloost::helper::toString(_eventCounter) + " Received \"" + eventName + "\" on  \"" + getNode()->getLabel() + "\"",
                                                       gloost::vec4(1.0f),
                                                       "/mainapp/log");
    ++_eventCounter;
  }

}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


