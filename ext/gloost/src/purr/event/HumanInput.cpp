
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


// tnt_gui system includes
#include <purr/event/HumanInput.h>
#include <purr/model/PurrModel.h>
#include <purr/model/Node.h>
#include <purr/action/CallEvent.h>
#include <purr/action/PickNodes.h>
#include <purr/action/RemoveNodes.h>
#include <purr/event/common.h>


// gloost includes
#include <gloost/Matrix.h>
#include <gloost/human_input/MouseState.h>
#include <gloost/postal/common.h>


// cpp includes
#include <GL/glfw.h>

namespace purr
{

namespace event
{

/**
  \class   HumanInput

  \brief   defines mouse/key events and behavior to interact with a model

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2013
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////
    /*static*/ const unsigned HumanInput::pickingFlagPosition = 15u;
////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a HumanInput
  \remarks ...
*/

/*static*/
HumanInput::shared_ptr
HumanInput::create(std::shared_ptr<model::PurrModel> model)
{
	shared_ptr newInput(new HumanInput(model));
	return newInput;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

HumanInput::HumanInput(std::shared_ptr<model::PurrModel> model):
  _lastDownNode(nullptr),
  _lastPickedNode(nullptr),
  _lastDragNode(nullptr),
  _timeSinceLastNodeChange(0.0),
  _hoverNode(nullptr),
  _model(model),
  _keyChangeQueueMutex(),
  _keyChangeQueue()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
HumanInput::~HumanInput()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   performs an interaction step with the gui depending on the mousestate
  \remarks ...
*/

///*virtual*/
void
HumanInput::frameStep(const std::shared_ptr<const gloost::human_input::MouseState>& mouseState,
                      const gloost::Matrix& viewMatrix)
{
//  static std::mutex mutex;
//  std::lock_guard<std::mutex> s_lock(mutex);
//

  if (!_model)
  {
    std::cerr << std::endl << "D'OH from HumanInput::frameStep(): " << this;
    std::cerr << std::endl << "          I have no model ...";
    return;
  }

  // create a GenericEventData of the mouseState to send it around
  auto theNewGlobalMouseState = event::GenericEventData<gloost::human_input::MouseState>::create( *(mouseState.get()) );

  // additionally put the mouseState into the model
  _model->setCurrentMouseState(theNewGlobalMouseState->getContent());

  gloost::mathType mouseSpeed = mouseState->getSpeed().length();
  bool mouseIsMoving = mouseSpeed > GLOOST_MATHTYPE_MIN_EPSILON;


  // # 0. send keyboard changes
  {
    std::lock_guard<std::mutex> lock(_keyChangeQueueMutex);
    while (_keyChangeQueue.size())
    {
      auto keyStateChange = _keyChangeQueue.front();
      auto keyStateChangeEventData = event::GenericEventData<KeyStateChange>::create( keyStateChange );

      _keyChangeQueue.pop();

//      std::cerr << std::endl << "sending: " << keyStateChange._keyId;

      if (keyStateChange._state)
      {
        action::CallEvent::callPreorder(_model->getRootNode(),
                                        purr::event::onKeyDown,
                                        keyStateChangeEventData);
      }
      else
      {
        action::CallEvent::callPreorder(_model->getRootNode(),
                                        purr::event::onKeyUp,
                                        keyStateChangeEventData);
      }
    }
  }

  // ## 1. "enterFrame" on ALL nodes
  action::CallEvent::callPreorder(_model->getRootNode(), purr::event::onEnterFrame);

  // pick to determine nodes currently underneath the mouse
  auto pickNodesAction = action::PickNodes::create();
  pickNodesAction->beginInorder(_model->getRootNode(), mouseState->getPosition());
  auto& pickedNodes = pickNodesAction->getPickedNodes();

  model::Node::shared_ptr currentlyPickedNode = nullptr;
  if (pickedNodes.size())
  {
    currentlyPickedNode = pickedNodes.back();
  }

  // ## 2. onMouseEnter/onMouseLeave
  if (currentlyPickedNode != _lastPickedNode)
  {
    action::CallEvent::call (_lastPickedNode,     purr::event::onMouseLeave, theNewGlobalMouseState);
    action::CallEvent::call (currentlyPickedNode, purr::event::onMouseEnter, theNewGlobalMouseState);
  }

  // ## 3. mouseUp
  if (mouseState->getButtonUpEvent(GLOOST_MOUSESTATE_BUTTON0))
  {
//    if (currentlyPickedNode)
    {
      // if it's NOT the same node as picked with mousedown
      if (_lastDownNode != currentlyPickedNode)
      {
        // call onMouseUpOutside on the last node picked with mousedown
        action::CallEvent::call(_lastDownNode, purr::event::onMouseUpOutside, theNewGlobalMouseState);
        action::CallEvent::call(currentlyPickedNode, purr::event::onMouseDrop, theNewGlobalMouseState);
      }
      else
      {
        // call onMouseUp on last node
        action::CallEvent::call(currentlyPickedNode, purr::event::onMouseUp, theNewGlobalMouseState);
      }
    }
    _lastDownNode = nullptr;
  }


  // ## 4. onDrag

  // end
  if (_lastDragNode && (mouseState->getButtonUpEvent(GLOOST_MOUSESTATE_BUTTON0) || !mouseIsMoving))
  {
    action::CallEvent::call(_lastDragNode, purr::event::onEndMouseDrag, theNewGlobalMouseState);
    _lastDragNode = nullptr;
  }


  // init
  if (!_lastDragNode && mouseState->getButtonState(GLOOST_MOUSESTATE_BUTTON0) && mouseIsMoving)
  {
    if ( _lastDownNode )
    {
      _lastDragNode = _lastDownNode;
      action::CallEvent::call(_lastDragNode, purr::event::onBeginMouseDrag, theNewGlobalMouseState);
    }
  }

  // proceed
  if (_lastDragNode && mouseIsMoving)
  {
    action::CallEvent::call(_lastDragNode, purr::event::onMouseDrag, theNewGlobalMouseState);
  }




  // ## 5. mouse down and offer/revoke focus
  if (mouseState->getButtonDownEvent(GLOOST_MOUSESTATE_BUTTON0))
  {
    if (currentlyPickedNode)
    {

//      std::cerr << std::endl << "From HumanInput::frameStep(): " << this;
//      std::cerr << std::endl << "     label: " << currentlyPickedNode->getLabel();

      _lastDownNode = currentlyPickedNode;
      action::CallEvent::call(currentlyPickedNode, purr::event::onMouseDown, theNewGlobalMouseState);

      // offer focus if node isn't allready the focused one
//      if (_model->getFocusedNode() != (currentlyPickedNode))
//      {
////        action::CallEvent::call(currentlyPickedNode, purr::event::onOfferFocus);
//      }
    }
    else
    {
//      if (_lastDownNode)
      {
        // revoke
//        action::CallEvent::call(_lastDownNode, purr::event::onRevokeFocus);
        _lastDownNode = nullptr;
      }

    }
  }


  // ## 6. call mouseWithin for currently picked node
  action::CallEvent::call(currentlyPickedNode, purr::event::onMouseWithin, theNewGlobalMouseState);



  // ## 7. mouseHover
  // begin
  if (!_hoverNode && _timeSinceLastNodeChange > 45.0 && !mouseIsMoving)
  {
    _hoverNode = currentlyPickedNode;
    action::CallEvent::call(_hoverNode, purr::event::onBeginMouseHover, theNewGlobalMouseState);
  }

  // proceed
  if (_hoverNode)
  {
    action::CallEvent::call(_hoverNode, purr::event::onMouseHover, theNewGlobalMouseState);
  }

  // end
  if (currentlyPickedNode != _lastPickedNode || mouseIsMoving)
  {
    _timeSinceLastNodeChange = 0.0;
    if (_hoverNode)
    {
      action::CallEvent::call(_hoverNode, purr::event::onEndMouseHover, theNewGlobalMouseState);
      _hoverNode = nullptr;
    }
  }
  ++_timeSinceLastNodeChange;



  // ## 8. "exitFrame"
  action::CallEvent::callPreorder(_model->getRootNode(), purr::event::onExitFrame);


  // ....
//  if (mouseState->getButtonDownEvent(GLOOST_MOUSESTATE_BUTTON2))
//  {
//    if (currentlyPickedNode && currentlyPickedNode->getParentNode())
//    {
//      action::RemoveNodes::removeSubtree(currentlyPickedNode);
//    }
//  }


  _lastPickedNode = currentlyPickedNode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   glfw version of the key up/down event handler
  \remarks ...
*/

///*virtual*/
void
HumanInput::keyEventGlfw(int key, int state)
{
  std::lock_guard<std::mutex> lock(_keyChangeQueueMutex);

  if (!state || !_model)
  {
    return;
  }
  _keyChangeQueue.push(KeyStateChange(key, state));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Node which received the last mouseDown event
  \remarks ...
*/

std::shared_ptr<model::Node>
HumanInput::getLastMouseDownNode()
{
  return _lastDownNode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   enables/disables the mouse picking for a node using a travmask
  \remarks ...
*/
/*static*/
void
HumanInput::setPickingFlag(const std::shared_ptr<purr::model::Node>& node, bool flag)
{
  if(node)
  {
    gloost::BitMask travMask = gloost::BitMask(node->getTravMask());
    travMask.setFlag(HumanInput::pickingFlagPosition, flag);
    node->setTravMask(travMask);
  }
}


////////////////////////////////////////////////////////////////////////////////


} // namespace event
} // namespace purr



