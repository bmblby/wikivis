
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



#ifndef H_GLOOST_PARTICLEHANDLER
#define H_GLOOST_PARTICLEHANDLER


// gloost system includes
#include <gloost/Point3.h>


// cpp includes
#include <list>
#include <vector>
#include <boost/thread/mutex.hpp>



namespace gloost
{

  class Particle;


  //  Base class for handling particles an storing extrinsic state of a set of particles

class ParticleHandler
{
	public:

    typedef std::vector<Particle*>            ParticleContainer;
    typedef ParticleContainer::iterator       ParticleListIterator;
    typedef ParticleContainer::const_iterator ParticleListConstIterator;


    // class constructor
    ParticleHandler();

    // class destructor
    virtual ~ParticleHandler();


    // performs dynamics
    virtual void frameStep(float deltaTime = 1.0);


    // Add a sprite
    void addParticle(Particle* particle);
    void addParticleFront(Particle* particle);


    // remove one entry
    void removeParticle(Particle* particle);


    // deletes all particles
    virtual void clear();


    // returns the number of particles
    unsigned getNumParticles() const;

    // get List Iterators
    ParticleListIterator getParticlesBegin();
    ParticleListIterator getParticlesEnd();

    // get List Iterators
    ParticleListConstIterator getParticlesBegin() const;
    ParticleListConstIterator getParticlesEnd() const;

    // returns a reference to the particle list
    ParticleContainer& getParticles();
    const ParticleContainer& getParticles() const;


	protected:

    // container of particles
    ParticleContainer _particles;

    boost::mutex _particlesMutex;

};


} // gloost


#endif // H_GLOOST_PARTICLEHANDLER



