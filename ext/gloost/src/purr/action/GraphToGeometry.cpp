
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

// purr system includes
#include <purr/action/GraphToGeometry.h>
#include <purr/model/Node.h>

// cpp includes
#include <string>
#include <iostream>

namespace purr
{
namespace action
{

/**
  \class   GraphToGeometry

  \brief   Build Geometry to visualize a subtree of the purr graph

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    May 2016
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

GraphToGeometry::GraphToGeometry():
  ActionBase("GraphToGeometry"),
  _graphDepth(0u),
  _numNodes(0u),
  _rootVisNode(nullptr),
  _nodesPerLevel()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
GraphToGeometry::shared_ptr
GraphToGeometry::create()
{
	return shared_ptr(new GraphToGeometry());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
GraphToGeometry::~GraphToGeometry()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   rebuilds the graphical representation of the graph
  \param   ...
  \remarks ...
*/

void
GraphToGeometry::begin( const std::shared_ptr<model::Node>& startNode )
{
  if (!startNode) return;

  gloost::util::Timer timer;
  timer.start();

  std::cerr << std::endl << "From GraphToGeometry::begin(): " << this;
  std::cerr << std::endl << "     Starting with node: " << startNode;

  // build graph
  auto rootVisNode = visualize::VisNode::create(startNode);
  rootVisNode->setDepth(0u);
  buildGraph(rootVisNode, 0u);

  std::cerr << std::endl;
  std::cerr << std::endl << "     graphDepth: " << _graphDepth;
  std::cerr << std::endl << "     _numNodes:  " << _numNodes;
  std::cerr << std::endl << "     done in:    " << timer.getDurationInMilli() << " ms";

  // arrage
  arrangeNodes();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \param   ...
  \brief   rebuilds the graphical representation of the graph
  \remarks ...
*/

void
GraphToGeometry::buildGraph( const std::shared_ptr<visualize::VisNode>& currentNode, unsigned depth )
{
  auto purrNode = currentNode->getPurrNode();
  if (!purrNode) return;

  ++_numNodes;

  while ( _nodesPerLevel.size() < depth+1 )
  {
    _nodesPerLevel.push_back( std::vector< std::shared_ptr<visualize::VisNode > >() );
  }
  _nodesPerLevel[depth].push_back(currentNode);

  auto subNodes = purrNode->getSubnodes();
  const auto subDepth = depth+1u;
  for (const auto& subNode : subNodes)
  {
    auto subVisNode = visualize::VisNode::create(subNode);
    currentNode->attachSubnode(subVisNode);
    subVisNode->setDepth(subDepth);
    buildGraph(subVisNode, subDepth);
  }
  _graphDepth = gloost::math::max(_graphDepth, depth);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \param   ...
  \brief   rebuilds the graphical representation of the graph
  \remarks ...
*/

void
GraphToGeometry::arrangeNodes ( )
{
  static auto sizeForDepth = [](unsigned depth) { return 100.0f/(float)depth; };

  if (!_nodesPerLevel.size()) return;

  for (int l=_nodesPerLevel.size()-1; l!=-1; --l)
  {
    auto& nodesOfThisLevel = _nodesPerLevel[l];
    for (unsigned i=0u; i!=nodesOfThisLevel.size(); ++i)
    {
      auto node = nodesOfThisLevel[i];

      const auto size = sizeForDepth(l);
      node->setSize({size, size, 0.0});
      node->setPosition({i*size + 0.1, gloost::mathType(l), 0.0});
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace action
} // namespace purr
