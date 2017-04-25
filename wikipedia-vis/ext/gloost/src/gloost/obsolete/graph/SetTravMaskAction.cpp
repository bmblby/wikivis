
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

            gloost is being created by Felix Weiﬂig and Stephan Beck

     Felix Weiﬂig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/



// gloost system includes
#include <gloost/graph/SetTravMaskAction.h>

namespace gloost
{
namespace graph
{



////////////////////////////////////////////////////////////////////////////////


 /// class constructor

SetTravMaskAction::SetTravMaskAction():
  Action(),
  _currentCamera(0)
{

}

////////////////////////////////////////////////////////////////////////////////


 /// class constructor

SetTravMaskAction::SetTravMaskAction(int contextId):
  Action(contextId),
  _currentCamera(0)
{

}


////////////////////////////////////////////////////////////////////////////////


 /// class destructor

SetTravMaskAction::~SetTravMaskAction()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

void
SetTravMaskAction::startSetFlags (Node* startNode, bool value)
{
//  _currentCamera = camera;
  setFlags(startNode, value);
}


////////////////////////////////////////////////////////////////////////////////


 ///

void
SetTravMaskAction::setFlags (Node* myNode, bool value)
{
  // get Iterators
  Node::childListIterator myIterator    = myNode->getChildListBegin();
  Node::childListIterator myEndIterator = myNode->getChildListEnd();


  // Check travmasks
  if ( _travMask & myNode->getTravMask() )
  {
    myNode->getTravMask().setFlags(_workMask, value);

    // handle child all childs
    for(; myIterator != myEndIterator; myIterator++)
    {
      setFlags((*myIterator), value);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

//void
//SetTravMaskAction::startCullNothing (Node* startNode)
//{
//  _currentCamera = 0;
//  cullNothing(startNode);
//}


////////////////////////////////////////////////////////////////////////////////


 ///

//void
//SetTravMaskAction::cullNothing (Node *myNode)
//{
//  // get Iterators
//  Node::childListIterator myIterator    = myNode->getChildListBegin();
//  Node::childListIterator myEndIterator = myNode->getChildListEnd();
//
//  // handle this node
//  Core *theCore = 0;
//
//  // Check travmasks
//  if ( _travMask & myNode->getTravMask() )
//  {
//    if ( myNode->getCore() )
//    {
//      theCore = myNode->getCore();
//
//
//      /// set the current node of the action to this node
//      setCurrentNode(myNode);
//
//
//      /// Do the switch
//      if (theCore->getType() == GLOOST_CORE_LOD)
//      {
//        GeometryCore* geoCore = dynamic_cast<GeometryCore*>(theCore);
//        myNode->getTravMask().setFlags(_workMask, true);
//      }
//    }
//    else
//    {
//      std::cout << std::endl;
//      std::cout << std::endl << "Warning from FrustumCullingAction::cull (Node *myNode):" ;
//      std::cout << std::endl << "             This core is NULL! You should give every Node a";
//      std::cout << std::endl << "             core to avoid undefined behavior of your application.";
//      std::cout << std::endl << "             Use the gloost::PureCore for cores with no functionality.";
//      std::flush(std::cout);
//    }
//
//    // handle child all childs
//    for(; myIterator != myEndIterator; myIterator++)
//    {
//      cullNothing(*myIterator);
//    }
//  }
//}


///////////////////////////////////////////////////////////////////////////////




} // namespace graph
} // namespace gloost




