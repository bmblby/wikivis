
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
#include <purr/action/PickNodes.h>
#include <purr/action/CallEvent.h>
#include <purr/behavior/Activate.h>
#include <purr/model/Node.h>
#include <purr/event/HumanInput.h>

// gloost system includes
#include <gloost/human_input/MouseState.h>
#include <gloost/gloostHelper.h>
#include <gloost/MatrixStack.h>
#include <gloost/gloostMath.h>



// cpp includes
#include <string>
#include <iostream>



namespace purr
{


namespace action
{


/**
  \class   PickNodes

  \brief   base class of all action/traverser

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief
  \remarks ...
*/

/*static*/
PickNodes::shared_ptr
PickNodes::create()
{
	shared_ptr model(new PickNodes());
	return model;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

PickNodes::PickNodes():
  ActionBase("PickNodes"),
  _pickedNodes(),
  _travMask(GLOOST_BITMASK_ALL_UNSET)
{
  _travMask.setFlag(purr::event::HumanInput::pickingFlagPosition, true);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
PickNodes::~PickNodes()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   begins the traversal on a specific node
  \remarks ...
*/

/*virtual*/
void
PickNodes::beginInorder(const std::shared_ptr<model::Node>& startNode,
                        const gloost::Point3& worldSpaceCoord)
{
  gloost::MatrixStack modelMatrixStack;
  int depth = 0;
  _pickedNodes.clear();

  traverseInorder(startNode, worldSpaceCoord, modelMatrixStack, depth);


  // call activate on the deepest node
//  if (_pickedNodes.size())
//  {
////    action::CallEvent::call(_pickedNodes.back(), purr::behavior::Activate::setActive);
//  }

//  _pickedNodes.back()->getParentNode()->removeSubnode(_pickedNodes.back());



//  for (const auto& item : _pickedNodes)
//  {
//    action::CallEvent::call(item, "purr::behavior::Activate::activate");
//  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   begins the traversal on a specific node
  \remarks ...
*/

/*virtual*/
void
PickNodes::traverseInorder(const std::shared_ptr<model::Node>& currentNode,
                           const gloost::Point3& worldSpaceCoord,
                           gloost::MatrixStack& modelMatrixStack,
                           int currentDepth)
{
  if(!(currentNode->getTravMask() & _travMask))
  {
    return;
  }

  modelMatrixStack.push();
  {
    modelMatrixStack.translate(currentNode->getPosition()*-1.0);
    auto currentNodeCoord = modelMatrixStack.top() * worldSpaceCoord;
    currentNodeCoord[2] = 0.5;

    if (   gloost::math::betweenOrEqual(currentNodeCoord[0], 0.0,  currentNode->getSize()[0])
        && gloost::math::betweenOrEqual(currentNodeCoord[1], 0.0,  currentNode->getSize()[1]))
    {
      _pickedNodes.push_back(currentNode);

    }

    // call for all my subnodes
    for (const auto& subNode : currentNode->getSubnodes())
    {
      traverseInorder(subNode,
                      worldSpaceCoord,
                      modelMatrixStack,
                      currentDepth+1);
    }
  }
  modelMatrixStack.pop();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns an inorder vector with shared_ptr to the picked nodes
  \remarks ...
*/

/*virtual*/
std::vector<std::shared_ptr<model::Node> >&
PickNodes::getPickedNodes()
{
  return _pickedNodes;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns an inorder vector with shared_ptr to the picked nodes
  \remarks ...
*/

/*virtual*/
const std::vector<std::shared_ptr<model::Node> >&
PickNodes::getPickedNodes() const
{
  return _pickedNodes;
}

////////////////////////////////////////////////////////////////////////////////




















} // namespace model

} // namespace purr


