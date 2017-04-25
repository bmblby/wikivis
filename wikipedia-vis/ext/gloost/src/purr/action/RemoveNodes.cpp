
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
#include <purr/action/RemoveNodes.h>
#include <purr/model/Node.h>
#include <purr/behavior/BehaviorBase.h>

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
  \class   RemoveNodes

  \brief   Provides methodes to remove or/and clean up nodes or subtrees

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/




////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

RemoveNodes::RemoveNodes():
  ActionBase("RemoveNodes")
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a whole subtree
  \remarks ...
*/

/*static*/
void
RemoveNodes::removeSubtree(const std::shared_ptr<model::Node>& node)
{
  std::vector<std::shared_ptr<model::Node> > collectedNodes;
  gatherNodesPostorder(node, collectedNodes);

  // call for all my subnodes
  for (auto& subNode : collectedNodes)
  {
    subNode->removeBehaviors();
    if (node->getParentNode())
    {
      node->getParentNode()->removeSubnode(node);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a whole subtree
  \remarks ...
*/

/*static*/
void
RemoveNodes::gatherNodesPostorder(const std::shared_ptr<model::Node>& node,
                                  std::vector< std::shared_ptr<model::Node> >& collectedNodes)
{
  // call for all my subnodes
  for (const auto& subNode : node->getSubnodes())
  {
    gatherNodesPostorder(subNode, collectedNodes);
  }
  collectedNodes.push_back(node);
}

////////////////////////////////////////////////////////////////////////////////




















} // namespace model

} // namespace purr


