
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



#include <gloost/graph/Node.h>
#include <gloost/graph/Core.h>
#include <gloost/graph/CoreManager.h>
#include <gloost/graph/MatrixCore.h>

#include <gloost/Matrix.h>
//#include <changeListManager.h>



namespace gloost
{
namespace graph
{


/**
  \class   Node

  \brief   A Node of a scenegraph. Has a pointer to a Core

  \author  Felix Weiszig
  \date    Juni 2010
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor

  \remarks This will create a Node with a PureCore (Core without functionality)
*/

Node::Node():
  _childList(),
  _parent(0),
  _core(CoreManager::get()->getCoreReference(GLOOST_COREMANAGER_DEFAULT_PURECORE)),
  _travMask(GLOOST_BITMASK_ALL_SET)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor

  \param   core A pointer to a Core
  \remarks The reference counter of the Core will be increased by 1
*/

Node::Node (Core *core):
  _childList(),
  _parent(0),
  _core(core),
  _travMask(GLOOST_BITMASK_ALL_SET)
{
  _core->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor

  \remarks To use this constructor, the Core has to be added to the CoreManager
           by calling
           \code
           unsigned int gloost::CoreManager::addCoreAndAssignName(Core* core, const std::string& name)
           \endcode

  \param nameWithinCoreManager The name you have given the Core when calling
         gloost::CoreManager::addCoreAndAssignName(Core* core, const std::string& name)
*/

Node::Node (const std::string& nameWithinCoreManager):
  _childList(),
  _parent(0),
  _core(CoreManager::get()->getCoreReference(nameWithinCoreManager)),
  _travMask(GLOOST_BITMASK_ALL_SET)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor

  \remarks To use this constructor, the Core has to be added to the CoreManager
           by calling
           \code
           unsigned int gloost::CoreManager::addCore(Core* core)
           \endcode
           or
           \code
           unsigned int gloost::CoreManager::addCoreAndAssignName(Core* core, const std::string& name)
           \endcode

  \param   coreManagerId The Id you got in return when calling
           gloost::CoreManager::addCore(Core* core)
           or
           gloost::CoreManager::addCoreAndAssignName(Core* core, const std::string& name)
*/

Node::Node (unsigned int coreManagerId):
  _childList(),
  _parent(0),
  _core(CoreManager::get()->getCoreReference(coreManagerId)),
  _travMask(GLOOST_BITMASK_ALL_SET)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor

  \remarks ...
*/

Node::~Node()
{
  _core->dropReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief adds a child node

  \remarks ...
*/

void
Node::addChild (Node *myNode )
{
  _childList.push_back(myNode);
  myNode->setParent(this);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief removes a child node

  \remarks ...
*/

void
Node::removeChild (Node *myNode)
{
  _childList.remove(myNode);
  myNode->setParent(0);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns a Node::childListIterator pointing at .begin() of this Nodes child list

  \remarks ...
*/

Node::childListIterator
Node::getChildListBegin()
{
	return _childList.begin();
}


////////////////////////////////////////////////////////////////////////////////


  ///

Node::childListIterator
Node::getChildListEnd()
{
	return _childList.end();
}


////////////////////////////////////////////////////////////////////////////////


  /// get the childlist (be carefull with this one)

std::list<Node*>&
Node::getChildList()
{
  return _childList;
}


////////////////////////////////////////////////////////////////////////////////


  /// returns the childlist of the Node

const std::list<Node*>&
Node::getChildList() const
{
  return _childList;
}


////////////////////////////////////////////////////////////////////////////////


  /// returns the number of children of this node

unsigned int
Node::getNumChildren()
{
  return _childList.size();
}


////////////////////////////////////////////////////////////////////////////////


  ///

void
Node::setParent (Node* myNode)
{
  _parent = myNode;
}


////////////////////////////////////////////////////////////////////////////////


  ///

Node*
Node::getParent () const
{
  return _parent;
}


////////////////////////////////////////////////////////////////////////////////


  /// set nodes core

void
Node::setCore (Core *core)
{
  _core->dropReference();

  _core = core;
  _core->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


  /// set nodes core

void
Node::setCore (const std::string& nameWithinCoreManager)
{

  if (_core)
  {
    _core->dropReference();
  }

  _core = CoreManager::get()->getCoreReference(nameWithinCoreManager);
  _core->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


  /// set nodes core

void
Node::setCore (unsigned int coreManagerId)
{

  if (_core)
  {
    _core->dropReference();
  }

  _core = CoreManager::get()->getCoreReference(coreManagerId);
}


////////////////////////////////////////////////////////////////////////////////


  /// get nodes core

Core*
Node::getCore () const
{
	return _core;
}


////////////////////////////////////////////////////////////////////////////////


/// swaps the cores of this Node and another

void
Node::swapCores(Node* node)
{
  Core* tmpCore = _core;
  _core = node->_core;
  node->_core = tmpCore;
}


////////////////////////////////////////////////////////////////////////////////


  /// return matrix with nodes transformations in world coordinates

const Matrix
Node::toWorld() const
{
  if (getParent() == 0)
  {
    /// handle root with matrixCore
    if (_core->getType() == GLOOST_CORE_MATRIX)
    {
      MatrixCore* matrixCore = dynamic_cast<MatrixCore*>(_core);
      return matrixCore->getMatrix();
    }
    /// handle root WITHOUT matrixCore
    else
    {
      Matrix I;
      I.setIdentity();
      return I;
    }
  }
  else
  {
    /// handle node with matrixCore
    if (_core->getType() == GLOOST_CORE_MATRIX)
    {
      MatrixCore* theMatrixCore = dynamic_cast<MatrixCore*>(_core);
      return (getParent()->toWorld() * theMatrixCore->getMatrix());
    }
    /// handle subnode without matrixCore
    else
    {
      return getParent()->toWorld();
    }
  }
}


////////////////////////////////////////////////////////////////////////////////


  ///

BitMask&
Node::getTravMask (  )
{
  return _travMask;
}


////////////////////////////////////////////////////////////////////////////////


  ///

const BitMask&
Node::getTravMask (  ) const
{
  return _travMask;
}

////////////////////////////////////////////////////////////////////////////////


  /// set travMask

void
Node::setTravMask(const BitMask& travMask)
{
  _travMask = travMask;
}


////////////////////////////////////////////////////////////////////////////////


  ///



} // namespace graph
} // namespace gloost



