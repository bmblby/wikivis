
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
#include <gloost/graph/MatrixCore.h>
#include <gloost/graph/IntersectAction.h>
#include <gloost/graph/VboCore.h>

#include <gloost/Matrix.h>
#include <gloost/Ray.h>
#include <gloost/obsolete/Vbo.h>
#include <gloost/Mesh.h>



// cpp includes



namespace gloost
{
namespace graph
{



////////////////////////////////////////////////////////////////////////////////


 /// class constructor

IntersectAction::IntersectAction():
  Action(),
  _hitNode(0),
  _hitRay()
{

}


////////////////////////////////////////////////////////////////////////////////


 /// class destructor

IntersectAction::~IntersectAction()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


 /// Run and return last Node visited

/* virtual */
bool
IntersectAction::start (Node *node, Ray& pickRayFromCamera)
{
  return intersect(node, pickRayFromCamera);
}


////////////////////////////////////////////////////////////////////////////////


 ///

bool
IntersectAction::intersect (Node *node, Ray& pickRayFromCamera)
{
  // get Iterators
  Node::childListConstIterator myIterator    = node->getChildListBegin();
  Node::childListConstIterator myEndIterator = node->getChildListEnd();

  // handle this node
  Core* theCore;

  // Check travmasks
  if (_travMask & node->getTravMask())
  {

    theCore   = node->getCore();
    Ray myRay = pickRayFromCamera;

    /// if there is a core
    if (theCore)
    {
      /// Handle geoCore
      if (theCore->getType() == GLOOST_CORE_VBO)
      {
        VboCore* vboCore = dynamic_cast<VboCore*>(theCore);
        if (vboCore->getVbo()->getMesh()->getBoundingBox().intersect(myRay))
        {

          double newDepth = myRay.getT()/Vector3(myRay.getDirection()).length2();
          double oldDepth = _hitRay.getT()/Vector3(_hitRay.getDirection()).length2();

          if (newDepth < oldDepth)
          {
            pickRayFromCamera = myRay;
            _hitRay  = myRay;
            _hitNode = node;
          }

        }
      }

      /// Handle matrixCore
      else if (theCore->getType() == GLOOST_CORE_MATRIX)
      {
        MatrixCore* mCore = dynamic_cast<MatrixCore*>(theCore);
        Matrix m = mCore->getMatrix();
        m.invert();
        myRay = m*myRay;
      }
    }
    else
    {
      std::cout << std::endl;
      std::cout << std::endl << "Warning from IntersectAction::intersect (Node *node, Ray& pickRayFromCamera)" ;
      std::cout << std::endl << "             This core is NULL! You should give every Node a";
      std::cout << std::endl << "             core to avoid undefined behavior of your application.";
      std::cout << std::endl << "             Use the gloost::PureCore for cores with no functionality.";
      std::flush(std::cout);
    }


    /// This variable is
    bool hitAChild = 0;

    // handle child all childs
    for(; myIterator != myEndIterator; myIterator++)
    {
      hitAChild = intersect(*myIterator, myRay);
    }

  }

  return _hitNode;
}

///////////////////////////////////////////////////////////////////////////////


  ///

Node*
IntersectAction::getHitNode() const
{
  return _hitNode;
}


///////////////////////////////////////////////////////////////////////////////


  ///

const Ray
IntersectAction::getHitRay() const
{
  return _hitRay;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
IntersectAction::reset()
{
  _hitNode = 0;
  _hitRay.setTMax();
}


///////////////////////////////////////////////////////////////////////////////

  ///



///////////////////////////////////////////////////////////////////////////////




} // namespace graph
} // namespace gloost



