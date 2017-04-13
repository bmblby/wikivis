
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
#include <purr/visualize/VisNode.h>

// cpp includes
#include <string>
#include <iostream>

namespace purr
{
namespace visualize
{

/**
  \class   VisNode

  \brief   A class template because I'm to lazy to write a class from scratch

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    May 2011
  \remarks Lazy mans way to create a new class ;-)
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

VisNode::VisNode(const std::shared_ptr< model::Node >&  purrNode):
  _purrNode(purrNode),
  _nodeDepth(0u),
  _parentVisNode(nullptr),
  _subVisNode(),
  _position(0.0,0.0,0.0),
  _size(1,1,1)
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
VisNode::shared_ptr
VisNode::create(const std::shared_ptr< model::Node >&  purrNode )
{
	return shared_ptr(new VisNode(purrNode));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
VisNode::~VisNode()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the VisNode
  \param   ...
  \remarks ...
*/

void
VisNode::setDepth(unsigned depth)
{
	_nodeDepth = depth;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the VisNode
  \param   ...
  \remarks ...
*/

unsigned
VisNode::getDepth()
{
	return _nodeDepth;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the VisNode
  \param   ...
  \remarks ...
*/

std::shared_ptr< model::Node >
VisNode::getPurrNode()
{
	return _purrNode;
}

////////////////////////////////////////////////////////////////////////////////

void
VisNode::attachSubnode(const shared_ptr& subNode)
{
  for (const auto& item : _subVisNode)
  {
    if (subNode == item) return;
  }
  _subVisNode.push_back(subNode);
  subNode->_parentVisNode = shared_from_this();
}

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<VisNode>
VisNode::getParent()
{
	return _parentVisNode;
}

////////////////////////////////////////////////////////////////////////////////

void
VisNode::setPosition( const gloost::Point3& pos )
{
	_position = pos;
}

////////////////////////////////////////////////////////////////////////////////

void
VisNode::setSize( const gloost::Vector3& size )
{
	_size = size;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace visualize
} // namespace purr


