
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
#include <gloost/Application.h>

namespace gloost
{

/**
  \class   Application

  \brief   Base class of an OpenGl application

  \author  Felix Weiszig
  \date    April 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \param   applicationTypeId id to distinguish or identify application typesid to
           distinguish or identify application types
  \remarks ...
*/

Application::Application(unsigned applicationTypeId):
  _applicationTypeId(applicationTypeId),
  _frameCounter(0)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Application::~Application()
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   initialises the application
  \remarks ...
*/

/*virtual*/
void
Application::init()
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   performes a frame iteration
  \param   deltaTime time since the last frame
  \remarks ...
*/

/*virtual*/
void
Application::frameStep(float deltaTime)
{
  _frameCounter += deltaTime;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   draws content
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Application::draw()
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   propergates window resizing through the application
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Application::resize(unsigned, unsigned)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   handles keyboard input
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Application::keyfunc(unsigned char, int, int)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   handles mouse movement
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Application::motionFunc(int mouse_h, int mouse_v)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   handles mouse events
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Application::mouseFunc(int /*button*/, int /*state*/, int /*mouse_h*/, int /*mouse_v*/)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   handles to much time
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Application::idleFunc()
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the type id
  \param   ...
  \remarks ...
*/

/*virtual*/
unsigned
Application::getApplicationTypeId() const
{
  return _applicationTypeId;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost
