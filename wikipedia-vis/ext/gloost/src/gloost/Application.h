
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



#ifndef H_GLOOST_APPLICATION
#define H_GLOOST_APPLICATION



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>


namespace gloost
{

  //  Base class of an OpenGl application

class Application
{
	public:

    Application(unsigned applicationTypeId = 0);
	  virtual ~Application();

    // init the Application
	  virtual void init();

	  // performes a frame iteration
	  virtual void frameStep(float deltaTime = 1.0);

	  // draws content
	  virtual void draw();


    // propergates window resizing through the application
    virtual void resize(unsigned width, unsigned height);

    // handles keyboard input
    virtual void keyfunc(unsigned char key, int x, int y);

    // handles mouse movement
    virtual void motionFunc(int mouse_h, int mouse_v);

    // handles mouse events
    virtual void mouseFunc(int button, int state, int mouse_h, int mouse_v);

    // handles to much time
    virtual void idleFunc();


    // returns the type id
    virtual unsigned getApplicationTypeId() const;


	protected:

    // id to distinguish or identify application types
    unsigned _applicationTypeId;

    // counts the number of frames
    float _frameCounter;


};


} // namespace gloost


#endif // H_GLOOST_APPLICATION


