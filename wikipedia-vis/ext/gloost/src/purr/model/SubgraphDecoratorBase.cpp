
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


// purr includes
#include <purr/model/SubgraphDecoratorBase.h>
#include <purr/model/Node.h>
#include <purr/model/PurrModel.h>
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

namespace model
{


/**
  \class   SubgraphDecoratorBase

  \brief    wraps a subgraph and decorates it with additional functionality

  \author  Felix Weiszig
  \date    february 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
//SubgraphDecoratorBase::shared_ptr
//SubgraphDecoratorBase::create()
//{
//	return shared_ptr(new SubgraphDecoratorBase());
//}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

SubgraphDecoratorBase::SubgraphDecoratorBase(const std::shared_ptr<model::PurrModel>& purrModel,
                                             const std::string& label,
                                             const std::string& type):
  _nodes(),
  _subgraphRootNode( model::Node::create(purrModel, "root"))
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

SubgraphDecoratorBase::~SubgraphDecoratorBase()
{
  removeFromTree();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the root node
  \remarks ...
*/

/*virtual */
std::shared_ptr<model::Node>
SubgraphDecoratorBase::getSubgraphRootNode()
{
  return _subgraphRootNode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the root node
  \remarks ...
*/

/*virtual */
std::shared_ptr<const model::Node>
SubgraphDecoratorBase::getSubgraphRootNode() const
{
  return _subgraphRootNode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a Node to the widget
  \remarks ...
*/

void
SubgraphDecoratorBase::addNode(const std::string& name, const std::shared_ptr<model::Node>& node)
{
  _nodes[name] = node;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Nodes this widget consists of
  \remarks ...
*/

SubgraphDecoratorBase::NodeContainer&
SubgraphDecoratorBase::getNodes()
{
  return _nodes;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Nodes this widget consists of
  \remarks ...
*/

const SubgraphDecoratorBase::NodeContainer&
SubgraphDecoratorBase::getNodes() const
{
  return _nodes;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a node by its given name within this decorator or a nullptr
  \remarks ...
*/

std::shared_ptr<Node>
SubgraphDecoratorBase::getNode(const std::string& nameWithinDecorator) const
{
  auto pos = _nodes.find(nameWithinDecorator);
  if (pos == _nodes.end())
  {
    return nullptr;
  }
  return pos->second;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a node as subnode to the root of this subgraph
  \remarks ...
*/

/*static*/
bool
SubgraphDecoratorBase::attachSubGraph(std::shared_ptr<Node> parentNode,  shared_ptr subgraph)
{
  if (parentNode && subgraph && subgraph->getSubgraphRootNode())
  {
    parentNode->addSubnode(subgraph->getSubgraphRootNode());
    return true;
  }
  return false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes the widget from the tree, i.e.: removes the widgetRoot from its parent
  \remarks ...
*/

void
SubgraphDecoratorBase::removeFromTree()
{
  if (_subgraphRootNode && _subgraphRootNode->getParentNode())
  {
    _subgraphRootNode->getParentNode()->removeSubnode(_subgraphRootNode);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the root node
  \remarks ...
*/

void
SubgraphDecoratorBase::setSubgraphRootNode(const std::shared_ptr<model::Node>& node)
{
  _subgraphRootNode = node;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace model
} // namespace purr


