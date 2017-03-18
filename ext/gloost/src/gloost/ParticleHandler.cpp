
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
#include <gloost/ParticleHandler.h>
#include <gloost/Particle.h>

// c++ includes
#include <algorithm>
#include <omp.h>



namespace gloost
{


/**
  \class   ParticleHandler

  \brief   Base class of particle handlers, storing extrinsic state of a set of particles

  \author  Felix Weiszig
  \date    March 2012
  \remarks ...
*/


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   class contructor
  \param   ...
  \remarks ...
*/

ParticleHandler::ParticleHandler():
  _particles(),
  _particlesMutex()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   class destructor
  \param   ...
  \remarks ...
*/

/*virtual*/
ParticleHandler::~ParticleHandler()
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Processes extrinsic state of the particle set, calls framestep on all
           particles and removes particles with no lifetime left
  \param   ...
  \remarks ...
*/

/*virtual*/
void
ParticleHandler::frameStep(float deltaTime)
{
//  omp_set_num_threads(4);
//  #pragma omp parallel for
  for (unsigned i=0; i<_particles.size(); ++i)
  {
    _particles[i]->frameStep(deltaTime);
  }

  ParticleListIterator particleIt    = _particles.begin();
  ParticleListIterator particleEndIt = _particles.end();

  for ( ; particleIt < particleEndIt; )
  {
    if ((*particleIt)->isAlive())
    {
      ++particleIt;
    }
    else
    {
      delete (*particleIt);
      particleIt    = _particles.erase(particleIt);
      particleEndIt = _particles.end();
    }
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a particle
  \param   ...
  \remarks ...
*/

void
ParticleHandler::addParticle(Particle* particle)
{
  boost::mutex::scoped_lock lock(_particlesMutex);
  _particles.push_back(particle);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Adds a particle
  \param   ...
  \remarks ...
*/

void
ParticleHandler::addParticleFront(Particle* /*particle*/)
{
//  _particles.push_front(particle);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   removes a particle
  \param   ...
  \remarks ...
*/

void
ParticleHandler::removeParticle(Particle* particle)
{
  ParticleListIterator pos = std::find(_particles.begin(), _particles.end(), particle);
  if (pos != _particles.end())
  {
    _particles.erase(pos);
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   clears all particles from this handler
  \param   ...
  \remarks ...
*/

void
ParticleHandler::clear()
{
  while (_particles.size())
  {
    delete _particles.back();
    _particles.pop_back();
  }
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   return number of particles
  \param   ...
  \remarks ...
*/

unsigned
ParticleHandler::getNumParticles() const
{
  return _particles.size();
}


///////////////////////////////////////////////////////////////////////////////


/// sort the particles by distance to a point

///*virtual*/
//bool
//ParticleHandler::sortByDistance(const gloost::Point3& cameraPoint)
//{
//  _cameraPosition = cameraPoint;
//  std::sort(_particles.begin(), _particles.end(), distCompareFunk);
//}
//
/////////////////////////////////////////////////////////////////////////////////
//
//
///*virtual*/
//bool
//ParticleHandler::distCompareFunk(Particle* left, Particle* right)
//{
//  float distL = (cameraPoint - left.position).length2();
//  float distR = (cameraPoint - right.position).length2();
//
//  return distL < distR;
//}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the begin iterator of the particle list
  \param   ...
  \remarks ...
*/

ParticleHandler::ParticleListIterator
ParticleHandler::getParticlesBegin()
{
  return _particles.begin();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the end iterator of the particle list
  \param   ...
  \remarks ...
*/

ParticleHandler::ParticleListIterator
ParticleHandler::getParticlesEnd()
{
  return _particles.end();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the begin iterator of the particle list
  \param   ...
  \remarks ...
*/

ParticleHandler::ParticleListConstIterator
ParticleHandler::getParticlesBegin() const
{
  return _particles.begin();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the end iterator of the particle list
  \param   ...
  \remarks ...
*/

ParticleHandler::ParticleListConstIterator
ParticleHandler::getParticlesEnd() const
{
  return _particles.end();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a reference to the particle list
  \param   ...
  \remarks ...
*/

ParticleHandler::ParticleContainer&
ParticleHandler::getParticles()
{
  return _particles;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a reference to the particle list
  \param   ...
  \remarks ...
*/

const ParticleHandler::ParticleContainer&
ParticleHandler::getParticles() const
{
  return _particles;
}

///////////////////////////////////////////////////////////////////////////////



} // /namespace gloost

