
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
#include <purr/action/TopologyList.h>
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
  \class   TopologyList

  \brief   Provides methodes to remove or/and clean up nodes or subtrees

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    June 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a whole subtree
  \remarks ...
*/

/*static*/
std::vector<TopologyList::ListEntry>
TopologyList::generateList(const std::shared_ptr<model::Node>& node)
{
  std::vector<ListEntry> collectedNodes;
  gatherNodesPreorder(node, collectedNodes, 0u);
  return collectedNodes;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a whole subtree
  \remarks ...
*/

/*static*/
void
TopologyList::gatherNodesPreorder(const std::shared_ptr<model::Node>& node,
                                  std::vector<TopologyList::ListEntry>& collectedNodes,
                                  unsigned depth)
{
  collectedNodes.push_back( ListEntry(node, depth) );
  for (const auto& subNode : node->getSubnodes())
  {
    gatherNodesPreorder(subNode, collectedNodes, depth+1);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   prints the list with additional indention for each depth level
  \remarks ...
*/

/*static*/
void
TopologyList::printList(std::vector<ListEntry>& list, int indentionWidth)
{
  std::cerr << std::endl;

  for (const auto& listEntry : list)
  {
    std::cerr << std::endl;
    for (unsigned i=0u; i!=listEntry._depth*indentionWidth; ++i)
    {
      std::cerr << " ";
    }
    std::cerr << listEntry._node->getLabel() << "(" << listEntry._node->getSubnodes().size() << ") ";
  }
}

////////////////////////////////////////////////////////////////////////////////




















} // namespace model

} // namespace purr


