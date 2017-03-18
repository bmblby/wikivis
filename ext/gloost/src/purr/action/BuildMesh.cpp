
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
#include <purr/action/BuildMesh.h>
#include <purr/model/Node.h>
#include <purr/behavior/BehaviorBase.h>
#include <purr/behavior/Activate.h>
#include <purr/behavior/DrawableEntity.h>

// gloost system includes
#include <gloost/human_input/MouseState.h>
#include <gloost/MatrixStack.h>



// cpp includes
#include <string>
#include <iostream>



namespace purr
{


namespace action
{


/**
  \class   BuildMesh

  \brief   Traverses the graph inorder and builds the vertex attributes to draw the Nodes

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
BuildMesh::shared_ptr
BuildMesh::create()
{
	shared_ptr model(new BuildMesh());
	return model;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

BuildMesh::BuildMesh():
  ActionBase("BuildMesh"),
  _geometryList()
{


}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
BuildMesh::~BuildMesh()
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
BuildMesh::begin(const std::shared_ptr<model::Node>& startNode)
{
  gloost::MatrixStack modelMatrixStack;
  int depth = 0;
  _geometryList.clear();

  if (!startNode)
  {
    return;
  }


  gatherGeometryInorder(startNode, modelMatrixStack, depth);
  decltype(_geometryList) listReversed(_geometryList.size());

  // reverse list (this is better done while rising from the recursion)
  unsigned j = 0;
  for (int i=(int)_geometryList.size()-1; i!=-1; --i)
  {
    listReversed[j++] = _geometryList[i];
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   begins the traversal on a specific node
  \remarks ...
*/

/*virtual*/
void
BuildMesh::gatherGeometryInorder(const std::shared_ptr<model::Node>& currentNode,
                                 gloost::MatrixStack& modelMatrixStack,
                                 int currentDepth)
{
  modelMatrixStack.push();
  {
    // get activation
    if (!currentNode)
    {
      return;
    }
    auto drawableEntityBehavior = behavior::DrawableEntity::castToThisType(currentNode->getBehavior(behavior::DrawableEntity::BehaviorName));
    if (drawableEntityBehavior)
    {
      auto entry = GeometryListEntry::create();
      entry->_worldSpaceAabb = modelMatrixStack.top() * currentNode->getAabb();

      modelMatrixStack.translate(currentNode->getPosition());

      entry->_modelMatrix = modelMatrixStack.top();
      entry->_node        = currentNode;
      entry->_nodeDepth   = currentDepth;

      // get activation
      entry->_activation = 0.0f;
      auto activateBehavior = behavior::Activate::castToThisType(currentNode->getBehavior(behavior::Activate::BehaviorName));
      if (activateBehavior)
      {
        entry->_activation = activateBehavior->getValue();
      }

      entry->_drawableEntityBehavior = drawableEntityBehavior;

//      gloost::Vector3 test = entry->_drawableEntityBehavior->getLabelOffset();
      _geometryList.push_back(entry);
    }
    else
    {
      modelMatrixStack.translate(currentNode->getPosition());
    }


    // call for all my subnodes
    for (const auto& subNode : currentNode->getSubnodes())
    {
      gatherGeometryInorder(subNode, modelMatrixStack, currentDepth+1);
    }
  }
  modelMatrixStack.pop();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a vector of GeometryListEntries
  \remarks ...
*/

/*virtual*/
BuildMesh::GeometryList&
BuildMesh::getGeometryList()
{
  return _geometryList;
}


////////////////////////////////////////////////////////////////////////////////




















} // namespace model

} // namespace purr


