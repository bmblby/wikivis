
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
#include <gloost/Scene.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


/**
  \class Scene
  \brief This class could serve as base class for a view of you application. It
  comes with virtual frameStep() methode to drive your application and a virtual
  draw function to draw itself. There are also a virtual init(), loadResources(),
  and unloadResources() methode to init you view.

  \author Felix Weiszig
  \date   December 2009

  \remarks Dont let the dogs bite you...
*/

Scene::Scene():
  PropertySet(),
  _resourcesLoaded(0),
  _sceneTime(0),
  _frameCounter(0)
{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

Scene::~Scene()
{

	// insert your code here

}


///////////////////////////////////////////////////////////////////////////////


  /// prepare to start / reset

/*virtual*/
bool
Scene::init()
{

  // insert code here ...
  return 1;
}


///////////////////////////////////////////////////////////////////////////////


  /// load resources for the scene

/*virtual*/
bool
Scene::loadResources()
{

  // insert code here ...
  _resourcesLoaded = true;
  return 1;
}


///////////////////////////////////////////////////////////////////////////////


  /// load resources for the scene

/*virtual*/
void
Scene::unloadResources()
{

  // insert code here ...

}


///////////////////////////////////////////////////////////////////////////////


  /// frame step

/*virtual*/
void
Scene::frameStep(float time)
{

  _sceneTime = time;
  ++_frameCounter;

}


///////////////////////////////////////////////////////////////////////////////


  /// draw

/*virtual*/
void
Scene::draw()
{

  // insert code here ...

}


///////////////////////////////////////////////////////////////////////////////


  /// returns true if resources ar loaded

bool
Scene::resourcesLoaded() const
{
  return _resourcesLoaded;
}


///////////////////////////////////////////////////////////////////////////////



} // namespace gloost


