
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



#ifndef GLOOST_SCENE_H
#define GLOOST_SCENE_H



// gloost system includes
#include <gloost/PropertySet.h>



// cpp includes
#include <string>



namespace gloost
{


  ///  Scene base class with prepare, frameStep and draw methode

class Scene: public PropertySet
{
	public:
    /// class constructor
    Scene();

    /// class destructor
	  virtual ~Scene();


	  /// prepare to start or reset to start
	  virtual bool init();


	  /// load resources for the scene
	  virtual bool loadResources();


	  /// load resources for the scene
	  virtual void unloadResources();


	  /// frame step
	  virtual void frameStep(float time = 0);


	  /// draw
	  virtual void draw();


	  /// returns true if resources ar loaded
	  bool resourcesLoaded() const;


	protected:

    /// indicates that the resources are loaded
    bool _resourcesLoaded;

    ///
    float _sceneTime;

    ///
    unsigned int _frameCounter;


};


} // namespace gloost


#endif // GLOOST_SCENE_H


