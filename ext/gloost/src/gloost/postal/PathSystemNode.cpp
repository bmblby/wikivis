
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
*/



#include <gloost/postal/PathSystemNode.h>

#include <iostream>
#include <algorithm>



namespace gloost
{
namespace postal
{



/**
  \class PathSystemNode

  \brief ...

  \author  Felix Weiszig
  \date    June 2013
  \remarks ...
*/


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a PathSystemNode and returns a std::shared_ptr to it
  \remarks ...
*/

/*static*/
std::shared_ptr<PathSystemNode>
PathSystemNode::create(const std::string& label,
                       const std::string& typeString,
                       gloost::gloostId itemGid)
{
  return std::shared_ptr<PathSystemNode>(new PathSystemNode(label,
                                                            typeString,
                                                            itemGid));
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

PathSystemNode::PathSystemNode(const std::string& label,
                               const std::string& typeString,
                               gloost::gloostId itemGid):
  _parent(),
  _children(),
  _label(label),
  _typeString(typeString),
  _itemGid(itemGid)
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

PathSystemNode::~PathSystemNode()
{
  for (auto child : _children)
  {
    child.second = nullptr;
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a child note
  \remarks ...
*/

bool
PathSystemNode::addChild(std::shared_ptr<PathSystemNode> node)
{
  // do nothing if the note is already a child of this one
  if (_parent.lock().get() && _parent.lock().get() == node->_parent.lock().get())
  {
    std::cerr << std::endl << "(D'OH):     PathSystemNode::addChild()";
    std::cerr << std::endl << "            Node is already connected to this parent";
    return false;
  }


  // allow only one child for each label
  if (_children.find(node->getLabel()) != _children.end())
  {
    std::cerr << std::endl << "(D'OH):     PathSystemNode::addChild()";
    std::cerr << std::endl << "            Node is already connected to this parent";
    return false;
  }

  if (node->getParent())
  {
    node->getParent()->removeChild(node);
  }

  // set new parent
  node->setParent(this->shared_from_this());

  // add to children
  _children[node->getLabel()] = node;

  return true;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a child note
  \remarks ...
*/

bool
PathSystemNode::removeChild(std::shared_ptr<PathSystemNode> node)
{
  auto pos = _children.find(node->getLabel());
  if (pos != _children.end())
  {
    pos->second->setParent(nullptr);
    _children.erase(pos);
    return true;
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the parent of this node
  \remarks ...
*/

void
PathSystemNode::setParent(std::shared_ptr<PathSystemNode> node)
{
  _parent = node;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns nodes parent or a nullptr
  \remarks ...
*/

std::shared_ptr<PathSystemNode>
PathSystemNode::getParent() const
{
  return _parent.lock();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a child note with a particular label or a null_ptr if the child node was not found
  \remarks ...
*/

std::shared_ptr<PathSystemNode>
PathSystemNode::getChild(const std::string& childsLabel) const
{
  auto pos = _children.find(childsLabel);
  if (pos == _children.end())
  {
    return nullptr;
  }
  return pos->second;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a std::vector of child nodes
  \remarks ...
*/

PathSystemNode::ChildNodeContainer&
PathSystemNode::getChildren()
{
  return _children;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a std::vector of child nodes
  \remarks ...
*/

const PathSystemNode::ChildNodeContainer&
PathSystemNode::getChildren() const
{
  return _children;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   add a subscriber to this PathSystemNode
  \remarks ...
*/

/*virtual*/
bool
PathSystemNode::addSubscriber(std::shared_ptr<Subscriber> subscriber)
{
  if ( _subscribers.find(subscriber) == _subscribers.end()  )
  {
    _subscribers.insert(subscriber);
    return true;
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   remove a subscriber from this PathSystemNode
  \remarks ...
*/

/*virtual*/
bool
PathSystemNode::removeSubscriber(std::shared_ptr<Subscriber> subscriber)
{
  auto pos = _subscribers.find(subscriber);
  if (pos == _subscribers.end())
  {
    return false;
  }
  _subscribers.erase(pos);
  return true;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns true if this PathSystemNode has subscriber in it
  \remarks ...
*/

/*virtual*/
bool
PathSystemNode::hasSubscriber(std::shared_ptr<Subscriber> subscriber) const
{
  auto pos = _subscribers.find(subscriber);
  if (pos == _subscribers.end())
  {
    return false;
  }
  return true;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a container with the Subscribers of this node
  \remarks ...
*/

/*virtual*/
const PathSystemNode::SubscriberContainer&
PathSystemNode::getSubscribers() const
{
  return _subscribers;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the label of the node
  \remarks ...
*/

const std::string&
PathSystemNode::getLabel() const
{
  return _label;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief    sets the label of this node
  \remarks ...
*/

void
PathSystemNode::setLabel(const std::string& label)
{
  _label = label;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the type string of this node
  \remarks ...
*/

const std::string&
PathSystemNode::getTypeString() const
{
  return _typeString;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief    sets the type string of this node
  \remarks ...
*/

void
PathSystemNode::setTypeString(const std::string& typeString)
{
  _typeString = typeString;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the item gid of this node
  \remarks ...
*/

gloost::gloostId
PathSystemNode::getItemGid() const
{
  return _itemGid;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the item gid of this node
  \remarks ...
*/

void
PathSystemNode::setItemGid(gloost::gloostId gid)
{
  _itemGid = gid;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the depth of this node within a tree
  \remarks ...
*/

unsigned
PathSystemNode::getDepth() const
{
  PathSystemNode::shared_ptr node = this->getParent();
  unsigned depth = 0u;

  while (node)
  {
    node = node->getParent();
    ++depth;
  }

  return depth;
}


///////////////////////////////////////////////////////////////////////////////


} // namespace postal
} // namespace gloost



