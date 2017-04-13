
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


#ifndef H_PURR_EVENT_COMMON
#define H_PURR_EVENT_COMMON


// purr includes
//#include<purr/event/EventData.h>
#include<purr/event/GenericEventData.h>


// cpp includes
#include <string>
#include <memory>
#include <unordered_set>




namespace purr
{
namespace event
{

  /**
    \namespace purr::event
    \brief     Defines static variables as name for common events. Provides an adapter for input
               events like from mouse or keyboard

    \author    Felix Weiszig, Dynardo GmbH Weimar
    \date      February 2014
    \remarks ...
  */

  typedef std::string EventId;

  // mouse releated
  const static std::string onMouseDown(       "purr::event::onMouseDown");
  const static std::string onMouseUp(         "purr::event::onMouseUp");
  const static std::string onMouseUpOutside(  "purr::event::onMouseUpOutside");
  const static std::string onMouseEnter(      "purr::event::onMouseEnter");
  const static std::string onMouseLeave(      "purr::event::onMouseLeave");
  const static std::string onBeginMouseDrag(  "purr::event::onBeginMouseDrag");
  const static std::string onEndMouseDrag(    "purr::event::onEndMouseDrag");
  const static std::string onMouseDrop(       "purr::event::onMouseDrop");
  const static std::string onBeginMouseHover( "purr::event::onBeginMouseHover");
  const static std::string onEndMouseHover(   "purr::event::onEndMouseHover");

  const static std::string onKeyDown(         "purr::event::onKeyDown");
  const static std::string onKeyUp(           "purr::event::onKeyUp");

  // frame wise
  const static std::string onEnterFrame(      "purr::event::onEnterFrame");
  const static std::string onExitFrame(       "purr::event::onExitFrame");
  const static std::string onMouseWithin(     "purr::event::onMouseWithin");
  const static std::string onMouseDrag(       "purr::event::onMouseDrag");
  const static std::string onMouseHover(      "purr::event::onMouseHover");

  // tree releated
  const static std::string onAddSubnode(     "purr::event::onAddSubnode");
  const static std::string onRemoveSubnode(  "purr::event::onRemoveSubnode");
  const static std::string onSetParentNode(  "purr::event::onSetParentNode");
  const static std::string onAddSibling(     "purr::event::onAddSibling");
  const static std::string onRemoveSibling(  "purr::event::onRemoveSibling");

  // Node releated
  const static std::string onChangePosition(       "purr::event::onChangePosition");
  const static std::string onChangeSize(           "purr::event::onChangeSize");
  const static std::string onParentChangePosition( "purr::event::onParentChangePosition");
  const static std::string onParentChangeSize(     "purr::event::onParentChangeSize");
  const static std::string onSubnodeChangePosition("purr::event::onSubnodeChangePosition");
  const static std::string onSubnodeChangeSize(    "purr::event::onSubnodeChangeSize");

  // gui releated
  /*
    This is mainly for the textinput / behaviors listening to keyboard events. Concept is, a node
    gets an offer to be in focus ( may be if onMouseDown has occured )
  */
  const static std::string onOfferFocus(  "purr::event::onOfferFocus");
  const static std::string onRevokeFocus( "purr::event::onRevokeFocus");


} // namespace event
} // namespace purr


#endif // H_PURR_EVENT_COMMON
