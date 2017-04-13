
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

// libpurr includes
#include <purr/model/Node.h>
#include <purr/model/PurrModel.h>
#include <purr/action/CallEvent.h>
#include <purr/event/common.h>
#include <purr/behavior/BehaviorBase.h>
#include <purr/behavior/DrawableEntity.h>
#include <purr/behavior/Activate.h>

// gloost includes
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/BoundingBox.h>

#include <iostream>
#include <algorithm>

namespace purr
{
namespace model
{

/**
  \class Node

  \brief ...

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
Node::shared_ptr
Node::create(const std::shared_ptr<PurrModel>& purrMode, const std::string& label,
                const std::string& typeString, gloost::gloostId itemGid)
{
  auto instance = shared_ptr(new Node(purrMode, label, typeString, itemGid));

  // add default behaviors
//  instance->addBehavior( purr::behavior::Activate::create());

  return instance;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

Node::Node(const std::shared_ptr<PurrModel>& purrModel,
                 const std::string& label,
                 const std::string& typeString,
                 gloost::gloostId   itemGid):
  _purrModel(purrModel),
  _label(label),
  _typeString(typeString),
  _itemGid(itemGid),
  _behaviorsByCallOrder(),
  _behaviorsByName(),
  _parentNode(),
  _subNodes(),
  _aabb({0.0,0.0,0.0}, {1.0,1.0,1.0}),
  _travMask(GLOOST_BITMASK_ALL_SET)
{

}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

Node::~Node()
{
 if (getParentNode())
 {
   getParentNode()->removeSubnode(shared_from_this());
 }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the assigned PurrModel instance or a nullptr
  \remarks ...
*/

std::shared_ptr<PurrModel>
Node::getPurrModel()
{
  return _purrModel.lock();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the assigned PurrModel instance or a nullptr
  \remarks ...
*/

std::shared_ptr<const PurrModel>
Node::getPurrModel() const
{
  if (_purrModel.expired())
  {
    return nullptr;
  }
  return _purrModel.lock();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds a subnode
  \remarks ...
*/

void
Node::addSubnode(const Node::shared_ptr& node)
{
  if(!node)
  {
    return;
  }

  if (node->getParentNode())
  {
    node->getParentNode()->removeSubnode(node);
  }

  node->setParentNode(shared_from_this());
  _subNodes.push_back(node);


  // call corresponding event
  auto eventData = event::GenericEventData<model::Node>::create(node);
  action::CallEvent::call(shared_from_this(),
                              purr::event::onAddSubnode,
                              eventData);

  action::CallEvent::callSiblings(node,
                                  purr::event::onAddSibling,
                                  eventData);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates Node, adds it as subnode and returns a shared ptr to it
  \remarks ...
*/

Node::shared_ptr
Node::addSubnode()
{
  auto newNode = create(getPurrModel());
  addSubnode(newNode);
  return newNode;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates Node, adds it as subnode and returns a shared ptr to it
  \remarks ...
*/

Node::shared_ptr
Node::addSubnode(const std::string& label)
{
  auto newNode = addSubnode();
  newNode->setLabel(label);
  return newNode;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes a subnode
  \remarks ...
*/

bool
Node::removeSubnode(const Node::shared_ptr& node)
{
  if(!node)
  {
    return false;
  }
  std::vector<shared_ptr>::iterator pos = std::find(_subNodes.begin(), _subNodes.end(), node);
  if (pos == _subNodes.end())
  {
    return false;
  }

  (*pos)->setParentNode(nullptr);

  auto subnode = (*pos);

  _subNodes.erase(pos);


  auto eventData = event::GenericEventData<model::Node>::create(subnode);
  action::CallEvent::call(shared_from_this(),
                              purr::event::onRemoveSubnode,
                              eventData);

  action::CallEvent::callSubnodes(shared_from_this(),
                                  purr::event::onRemoveSibling,
                                  eventData);

  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes all subnodes
  \remarks ...
*/

void
Node::removeAllSubnodes()
{
  for (const auto& node : _subNodes)
  {
    auto eventData = event::GenericEventData<model::Node>::create(node);


    action::CallEvent::call(shared_from_this(),
                                purr::event::onRemoveSubnode,
                                eventData);

    action::CallEvent::callSubnodes(shared_from_this(),
                                    purr::event::onRemoveSibling,
                                    eventData);

    node->setParentNode(nullptr);
  }
  _subNodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the parent of this node
  \remarks ...
*/

void
Node::setParentNode(const Node::shared_ptr& node)
{
  _parentNode = node;

  // call corresponding event
  action::CallEvent::call(shared_from_this(),
                              purr::event::onSetParentNode,
                              event::GenericEventData<model::Node>::create(node));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns nodes parent or a nullptr
  \remarks ...
*/

Node::shared_ptr
Node::getParentNode() const
{
  if (_parentNode.expired())
  {
    return nullptr;
  }
  return _parentNode.lock();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns this nodes position within parents vector of subnodes, return -1 if
*/

int
Node::getIndexWithinParentNode() const
{
  if (isRoot())
  {
    return -1;
  }

  std::vector<shared_ptr> siblings = _parentNode.lock()->getSubnodes();
  auto thisShared = shared_from_this();

  for (unsigned i=0u; i!=siblings.size(); ++i)
  {
    if (siblings[i] == thisShared)
    {
      return i;
    }
  }

  return -1;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a child note with a particular position within this nodes subnodes
  \remarks ...
*/

Node::shared_ptr
Node::getSubnode(unsigned index)
{
  if (index < _subNodes.size())
  {
    return _subNodes[index];
  }
  return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a child note with a particular position within this nodes subnodes
  \remarks ...
*/

Node::shared_ptr
Node::getSubnodeByLabel(const std::string& label)
{
  Node::shared_ptr node = nullptr;
  for (const auto& subnode : _subNodes)
  {
    if(label == subnode->getLabel())
    {
      node = subnode;
    }
  }
  return node;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a std::vector of child nodes
  \remarks ...
*/

std::vector<Node::shared_ptr>&
Node::getSubnodes()
{
  return _subNodes;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a std::vector of child nodes
  \remarks ...
*/

const std::vector<Node::shared_ptr>&
Node::getSubnodes() const
{
  return _subNodes;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   moves a subnode to the last position in the SubnodeContainer of this node
  \remarks ...
*/

void
Node::moveToSubNodesEnd(const shared_ptr& subnode)
{
  if (!_subNodes.size()) return;

  auto pos = 0u;
  for (; pos != _subNodes.size(); ++pos)
  {
    if (_subNodes[pos] == subnode) break;
  }
  // not found or already at the end
  if (pos == _subNodes.size() || pos == _subNodes.size()-1) return;

  for (unsigned i=pos; i!=_subNodes.size()-1; ++i)
  {
    _subNodes[i] = _subNodes[i+1];
  }
  _subNodes[_subNodes.size()-1] = subnode;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns true if this node is the a root
  \remarks ...
*/

bool
Node::isRoot() const
{
  return !(bool)_parentNode.lock();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns true if this node is the a leaf
  \remarks ...
*/

bool
Node::isLeaf() const
{
  return (bool)_subNodes.size();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   return the root node of the tree this node exists in
  \remarks ...
*/

Node::shared_ptr
Node::getRootNode()
{
  auto current = shared_from_this();
  while (current->getParentNode())
  {
    current = current->getParentNode();
  }
  return current;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the label of the node
  \remarks ...
*/

const std::string&
Node::getLabel() const
{
  return _label;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief    sets the label of this node
  \remarks ...
*/

void
Node::setLabel(const std::string& label)
{
  _label = label;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the type string of this node
  \remarks ...
*/

const std::string&
Node::getTypeString() const
{
  return _typeString;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief    sets the type string of this node
  \remarks ...
*/

void
Node::setTypeString(const std::string& typeString)
{
  _typeString = typeString;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the item gid of this node
  \remarks ...
*/

gloost::gloostId
Node::getItemGid() const
{
  return _itemGid;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the item gid of this node
  \remarks ...
*/

void
Node::setItemGid(gloost::gloostId gid)
{
  _itemGid = gid;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds a behavior to the Node
  \remarks ...
*/

void
Node::addBehavior(const std::shared_ptr<behavior::BehaviorBase>& behavior)
{
  if (_behaviorsByName.find(behavior->getName()) == _behaviorsByName.end())
  {
    _behaviorsByCallOrder.push_back(behavior);
    _behaviorsByName[behavior->getName()] = behavior;

    behavior->onAssignToNode(shared_from_this());
    behavior->onBeginBehavior();
  }
  else
  {
    std::cerr << std::endl << "D'OH from Node::addBehavior: " << this;
    std::cerr << std::endl << "     On Node: " <<  getLabel();
    std::cerr << std::endl << "     Could not add behavior with name: " << behavior->getName();
    std::cerr << std::endl << "     Because a behavior with the same name was allready found for this node.";
    std::cerr << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes a behavior from the Node
  \remarks ...
*/

void
Node::removeBehavior(const std::shared_ptr<behavior::BehaviorBase>& behavior)
{
  removeBehavior(behavior->getName());
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes a behavior from the Node
  \remarks ...
*/

void
Node::removeBehavior(const std::string& name)
{
  // from vector
  for (auto it = _behaviorsByCallOrder.begin(); it != _behaviorsByCallOrder.end(); )
  {
    if ((*it)->getName() == name)
    {
      it = _behaviorsByCallOrder.erase(it);
      continue;
    }
    ++it;
  }

  // from map
  auto it = _behaviorsByName.find(name);
  if (it != _behaviorsByName.end())
  {
    _behaviorsByName.erase(it);
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes all behaviors
  \remarks ...
*/

void
Node::removeBehaviors()
{
  for (auto& behavior : _behaviorsByCallOrder)
  {
    behavior->unmapAllEvents();
  }

  _behaviorsByCallOrder.clear();
  _behaviorsByName.clear();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of behaviors in call order
  \remarks ...
*/

Node::BehaviorVector&
Node::getBehaviorsByCallOrder()
{
  return _behaviorsByCallOrder;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of behaviors in call order
  \remarks ...
*/

const Node::BehaviorVector&
Node::getBehaviorsByCallOrder() const
{
  return _behaviorsByCallOrder;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a behavior by name or a nullptr
  \remarks ...
*/

std::shared_ptr<behavior::BehaviorBase>
Node::getBehavior(const std::string& behaviorName)
{
  auto iterator = _behaviorsByName.find(behaviorName);
  if (iterator != _behaviorsByName.end())
  {
    return iterator->second;
  }

  return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the depth of this node within a tree
  \remarks ...
*/

unsigned
Node::getDepth() const
{
  auto     node  = getParentNode();
  unsigned depth = 0u;

  while (node->getParentNode())
  {
    ++depth;
    node = node->getParentNode();
  }

  return depth;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   sorts the container of child nodes their label
  \remarks ...
*/

void
Node::sortChildrenByLabel()
{
  std::sort(_subNodes.begin(), _subNodes.end(),
            [](shared_ptr a, shared_ptr b) -> bool
  {
      return a->getLabel() < b->getLabel();
  });
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the travMask
  \remarks ...
*/

/*virtual*/
const gloost::BitMask&
Node::getTravMask() const
{
  return  _travMask;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the travMask
  \remarks ...
*/

/*virtual*/
void
Node::setTravMask(const gloost::BitMask& travMask)
{
   _travMask = travMask;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the position (lower left corner)
  \remarks ...
*/

void
Node::setPosition(const gloost::Point3& position)
{
  if (position == getPosition())
  {
    return;
  }

  auto eventData = event::GenericEventData<gloost::Point3>::create(position);

  _aabb.setPMax(position+_aabb.getSize());
  _aabb.setPMin(position);

  action::CallEvent::call(shared_from_this(),
                              event::onChangePosition,
                              eventData);

  action::CallEvent::callSubnodes(shared_from_this(),
                                  event::onParentChangePosition,
                                  eventData);


  action::CallEvent::call(getParentNode(),
                              event::onSubnodeChangePosition,
                              eventData);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the position (lower left corner)
  \remarks ...
*/

gloost::Point3
Node::getPosition() const
{
  return _aabb.getPMin();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   moves the node
  \remarks ...
*/

void
Node::move(const gloost::Vector3& speed)
{
  if (speed.length2() > GLOOST_MATHTYPE_MIN_EPSILON)
  {
    _aabb.getPMin() += speed;
    _aabb.getPMax() += speed;
    action::CallEvent::call(shared_from_this(), event::onChangePosition);
    action::CallEvent::callSubnodes(shared_from_this(), event::onParentChangePosition);
    action::CallEvent::call(getParentNode(), event::onSubnodeChangePosition );
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the position (lower left corner)
  \remarks ...
*/

void
Node::setSize(const gloost::Vector3& size)
{

  if (size == getSize())
  {
    return;
  }
  _aabb.setSize(size);
  action::CallEvent::call(shared_from_this(),     event::onChangeSize);
  action::CallEvent::callSubnodes(shared_from_this(), event::onParentChangeSize);
  action::CallEvent::call(getParentNode(), event::onSubnodeChangeSize );
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the position (lower left corner)
  \remarks ...
*/

gloost::Vector3
Node::getSize() const
{
  return _aabb.getSize();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the gloost::BoundingBox which defines the area of this entity
  \remarks ...
*/

const gloost::BoundingBox&
Node::getAabb() const
{
  return _aabb;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a matrix that transforms from local Node coords to worldCoords (the space the root node is in)
  \remarks ...
*/

gloost::Matrix
Node::getToWorldMatrix() const
{
  auto toWorldMatrix = gloost::Matrix::createTransMatrix(getPosition());
  auto parent = getParentNode();

  while(parent)
  {
    toWorldMatrix = toWorldMatrix*gloost::Matrix::createTransMatrix(parent->getPosition());
    parent = parent->getParentNode();
  }

  return toWorldMatrix;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   transforms a point from world space to Node space
  \remarks ...
*/

gloost::Point3
Node::fromWorldToNodeSpace(const gloost::Point3& point) const
{
  return getToWorldMatrix().inverted()*point;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   transforms a vector from world space to Node space
  \remarks ...
*/

gloost::Vector3
Node::fromWorldToNodeSpace(const gloost::Vector3& vector) const
{
  return getToWorldMatrix().inverted()*vector;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace model
} // namespace tami


