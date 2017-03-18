
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



#include <gloost/graph/DrawAction.h>



namespace gloost
{
namespace graph
{



////////////////////////////////////////////////////////////////////////////////


 /// class constructor

DrawAction::DrawAction():
  Action(),
  _currentCamera(0)
{

}

////////////////////////////////////////////////////////////////////////////////


 /// class constructor

DrawAction::DrawAction(int contextId):
  Action(contextId),
  _currentCamera(0)
{

}


////////////////////////////////////////////////////////////////////////////////


 /// class destructor

DrawAction::~DrawAction()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

/* virtual? */
void
DrawAction::start (Node* startNode, Camera* camera)
{
  _currentCamera = camera;
  draw(startNode);
}


////////////////////////////////////////////////////////////////////////////////


 ///

void
DrawAction::draw (Node *myNode)
{
  // get Iterators
  Node::childListIterator myIterator    = myNode->getChildListBegin();
  Node::childListIterator myEndIterator = myNode->getChildListEnd();

  // handle this node
  Core *theCore = 0;

  // Check travmasks
  if ( _travMask & myNode->getTravMask() )
  {


    if ( myNode->getCore() )
    {

      theCore = myNode->getCore();


      /// set the current node of the action to this node
      setCurrentNode(myNode);


      if ( _collector  )
      {
        _collector->beginCore( theCore );
      }

      theCore->enterCore(this);

    }
    else
    {

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
      std::cout << std::endl;
      std::cout << std::endl << "Warning from DrawAction::draw (Node *myNode)" ;
      std::cout << std::endl << "             Found a Node without core. You should give every Node a";
      std::cout << std::endl << "             core to avoid undefined behavior of your application.";
      std::cout << std::endl << "             Use the gloost::PureCore for cores with no functionality.";
      std::flush(std::cout);
#endif

      /// subgraphs below empty nodes will not be drawn
      return;
    }

    // handle child all childs
    for(; myIterator != myEndIterator; myIterator++)
    {
      draw(*myIterator);
    }

    if (myNode->getCore())
    {
      setCurrentNode(myNode);
      theCore->exitCore(this);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////




} // namespace graph
} // namespace gloost


