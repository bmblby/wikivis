
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



#ifndef H_GLOOST_PARTICLE
#define H_GLOOST_PARTICLE


// gloost system includes
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>



namespace gloost
{

  class BinaryBundle;


  //  base class of particles, storing intrensic state of a particle

class Particle
{
	public:

    // class constructor
    Particle(const Point3& position, float lifeTime);

    // class destructor
	  virtual ~Particle();


    // performs one step of the particles life
    virtual void frameStep(float deltaTime);


    // returns the position
	  Point3& getPosition();
	  const Point3& getPosition() const;

	  // sets the position
	  void setPosition(const Point3& position);


    // returns the lifetime
    float getCurrentLifeTime() const;

    // returns the lifetime
    float getMaxLifeTime() const;

    // returns ratio of currentLifetime/maxLifeTime
    float getLifeTimeRatio() const;

    // returns true if particle is alive
    virtual bool isAlive() const;

    // serializes particles attributes into a BinaryBundle to the current put position
    virtual void serialize(gloost::BinaryBundle* bundle) const;


  protected:

    Point3 _position;
    float  _currentLifeTime;
    float  _maxLifeTime;


};


} // namespace gloost


#endif // H_GLOOST_PARTICLE


