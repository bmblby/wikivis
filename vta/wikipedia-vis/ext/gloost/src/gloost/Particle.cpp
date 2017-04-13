
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
#include <gloost/Particle.h>
#include <gloost/BinaryBundle.h>


namespace gloost
{

/**
  \class   Particle

  \brief   base class of particles, storing intrensic state of a particle

  \author  Felix Weiszig
  \date    March 2012
  \remarks ...
*/

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   class constructor
  \param   ...
  \remarks ...
*/

Particle::Particle(const Point3& position, float lifeTime):
  _position(position),
  _currentLifeTime(0.0f),
  _maxLifeTime(lifeTime)
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   class constructor
  \param   ...
  \remarks ...
*/

/*virtual*/
Particle::~Particle()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   performs one step of the particles life
  \param   deltaTime duration of this step
  \remarks ...
*/

/*virtual*/
void
Particle::frameStep(float deltaTime)
{
  _currentLifeTime += deltaTime;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the position
  \param   ...
  \remarks ...
*/

/*virtual*/
Point3&
Particle::getPosition()
{
  return _position;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the position
  \param   ...
  \remarks ...
*/

/*virtual*/
const Point3&
Particle::getPosition() const
{
  return _position;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the position
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Particle::setPosition(const Point3& position)
{
  _position = position;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the current lifetime
  \param   ...
  \remarks ...
*/

/*virtual*/
float
Particle::getCurrentLifeTime() const
{
  return _currentLifeTime;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the max lifetime
  \param   ...
  \remarks ...
*/

/*virtual*/
float
Particle::getMaxLifeTime() const
{
  return _maxLifeTime;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns ratio of maxLifeTime/currentLifetime
  \param   ...
  \remarks ...
*/

/*virtual*/
float
Particle::getLifeTimeRatio() const
{
  return _currentLifeTime/_maxLifeTime;
}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the lifetime
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
Particle::isAlive() const
{
  return (_currentLifeTime < _maxLifeTime);
}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   serializes particles attributes into a BinaryBundle
  \param   ...
  \remarks ...
*/

/*virtual*/
void
Particle::serialize(gloost::BinaryBundle* bundle) const
{
  bundle->putFloat(_position[0]);
  bundle->putFloat(_position[1]);
  bundle->putFloat(_position[2]);
}


///////////////////////////////////////////////////////////////////////////////



} // namespace gloost


