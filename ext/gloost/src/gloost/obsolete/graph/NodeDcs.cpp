
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



// gloost includes
#include <gloost/graph/NodeDcs.h>
#include <gloost/graph/MatrixCore.h>

//#include <changeListManager.h>



// cpp includes



namespace gloost
{
namespace graph
{



////////////////////////////////////////////////////////////////////////////////


  ///

// class constructor
NodeDcs::NodeDcs():
  Dcs(),
  _core(new MatrixCore()),
  _node(new Node(_core))
{
  _core->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


  ///

// class constructor
NodeDcs::~NodeDcs()
{
  delete _node;
}


////////////////////////////////////////////////////////////////////////////////


  ///  add a child node

void
NodeDcs::addChild (Node *node )
{
  _node->addChild(node);
}


////////////////////////////////////////////////////////////////////////////////


  ///  add a child node

void
NodeDcs::addChild (NodeDcs *nodeDcs)
{
  _node->addChild(nodeDcs->getNode());
}


////////////////////////////////////////////////////////////////////////////////


  /// remove a child node

void
NodeDcs::removeChild (Node *node)
{
  _node->removeChild(node);
}


////////////////////////////////////////////////////////////////////////////////


  ///

void
NodeDcs::setParent (Node* node)
{
  _node->setParent(node);
}


////////////////////////////////////////////////////////////////////////////////


  ///

void
NodeDcs::setParent (NodeDcs* nodeDcs)
{
  _node->setParent(nodeDcs->getNode());
}


////////////////////////////////////////////////////////////////////////////////


  ///

Node*
NodeDcs::getParent () const
{
  return _node->getParent();
}


////////////////////////////////////////////////////////////////////////////////

  ///

Node*
NodeDcs::getNode () const
{
  return _node;
}


////////////////////////////////////////////////////////////////////////////////

  ///

MatrixCore*
NodeDcs::getMatrixCore () const
{
  return _core;
}


////////////////////////////////////////////////////////////////////////////////

  ///

void
NodeDcs::applyToCore ()
{
  /// this will validate the dcs and copy the result to the matrix of the MatrixCore

  if (isDirty())
  {
    _core->setMatrix(getValidMatrix());
  }
}


////////////////////////////////////////////////////////////////////////////////


} // namespace graph
} // namespace gloost

