
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



// gloost system includes
#include <gloost/graph/Action.h>


// cpp includes



namespace gloost
{

namespace graph
{


///////////////////////////////////////////////////////////////////////////////


 // class constructor

Action::Action():
  	_travMask(GLOOST_BITMASK_ALL_SET),
  	_workMask(GLOOST_BITMASK_ALL_UNSET),
  	_collector(0),
  	_contextId(0),
  	_currentNode(0)
{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


 // class constructor

Action::Action(int contextId):
  	_travMask(GLOOST_BITMASK_ALL_SET),
  	_workMask(GLOOST_BITMASK_ALL_UNSET),
  	_collector (0),
  	_contextId(contextId),
  	_currentNode(0)
{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


 /// class destructor

Action::~Action()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

/* virtual? */
void
Action::start (Node* startNode)
{
  std::cout << std::endl;
  std::cout << std::endl << "Message from Action::start (Node* startNode):" ;
  std::cout << std::endl << "             Methode void start(Node* startNode) is not";
  std::cout << std::endl << "             is not implemented on this Action. Look for";
  std::cout << std::endl << "             another start version." << startNode;
  std::flush(std::cout);
}


///////////////////////////////////////////////////////////////////////////////


 /// get the travMask

const BitMask&
Action::getTravMask () const
{
  return _travMask;
}


///////////////////////////////////////////////////////////////////////////////


 /// get the travMask

BitMask&
Action::getTravMask ()
{
  return _travMask;
}


///////////////////////////////////////////////////////////////////////////////


  /// get BitMask with flags the CullAction has to modify

const BitMask&
Action::getWorkMask() const
{
	return _workMask;
}


///////////////////////////////////////////////////////////////////////////////


  /// get BitMask with flags the CullAction has to modify

BitMask&
Action::getWorkMask()
{
	return _workMask;
}


///////////////////////////////////////////////////////////////////////////////


  /// set BitMask with flags the CullAction has to modify

void
Action::setWorkMask(const BitMask& workMask)
{
	_workMask = workMask;
}


///////////////////////////////////////////////////////////////////////////////


 /// get the collector

Collector*
Action::getCollector( )
{
	return _collector;
}


///////////////////////////////////////////////////////////////////////////////


 /// set the travMask

/* virtual */
void
Action::setTravMask ( const BitMask& travMask )
{
  _travMask = travMask;
}


///////////////////////////////////////////////////////////////////////////////


 /// set a collector

void
Action::setCollector( Collector *collector )
{
	_collector = collector;
}


///////////////////////////////////////////////////////////////////////////////


 /// get contextId

unsigned int
Action::getContextId() const
{
	return _contextId;
}

///////////////////////////////////////////////////////////////////////////////


 /// set contextId

void
Action::setContextId(unsigned int contextId)
{
	_contextId = contextId;
}


///////////////////////////////////////////////////////////////////////////////


  /// get contextId

Node*
Action::getCurrentNode() const
{
  return _currentNode;
}


///////////////////////////////////////////////////////////////////////////////


  /// set contextId

void
Action::setCurrentNode(Node* currentNode)
{
  _currentNode = currentNode;
}


///////////////////////////////////////////////////////////////////////////////


} // namespace graph
} // namespace gloost
