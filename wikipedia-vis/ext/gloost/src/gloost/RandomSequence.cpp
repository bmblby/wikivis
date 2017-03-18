
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


#include <gloost/RandomSequence.h>


namespace gloost
{

/**
  \class   RandomSequence

  \brief   Generator, generates a sequence of "random" numbers with given seed

  \remarks http://en.wikipedia.org/wiki/Linear_congruential_generator

  \author  Felix Weiszig
  \date    March 2012
  \remarks ...
*/

///////////////////////////////////////////////////////////////////////////////

/**
  \brief Class constructor
  \remarks ...
*/

RandomSequence::RandomSequence():
    _seed(999067),
    _currents(4, 999067),
    _current(999067),
    _a(2999047),
    _c(7452347),
    _m(9999083)
{
  reset();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief Class constructor
  \param seed seed of the random sequence
  \remarks ...
*/

RandomSequence::RandomSequence(unsigned seed):
    _seed(seed),
    _currents(4, seed),
    _current(seed),
    _a(299047),
    _c(752347),
    _m(9999083)
{
  reset();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief Class constructor
  \param seed seed of the random sequence
  \remarks ...
*/

RandomSequence::RandomSequence(unsigned seed, unsigned max):
    _seed(seed),
    _currents(4, seed),
    _current(seed),
    _a(299047),
    _c(752347),
    _m(max)
{
  reset();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns a random number between 0.0 ... 1.0
  \remarks ...
*/

mathType
RandomSequence::frand()
{
  iteration();
  return _current/(mathType)_m;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns a random number between -1.0 ... 1.0
  \remarks ...
*/

mathType
RandomSequence::crand()
{
  iteration();
  return (_current/(mathType)_m)*2.0 - 1;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns true if a random value between 0.0 and 1.0 is smaller as <probability>
  \remarks ...
*/

bool
RandomSequence::chance(float probability)
{
  iteration();
  return bool(frand() < probability);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns a random vector with components between 0.0 ... 1.0
  \remarks ...
*/

gloost::Vector3
RandomSequence::frandVector()
{
  gloost::Vector3 vector;

  iteration();
  vector[0] = _current/(mathType)_m;
  iteration();
  vector[1] = _current/(mathType)_m;
  iteration();
  vector[2] = _current/(mathType)_m;

  return vector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns a random vector with components between -1.0 ... 1.0
  \remarks ...
*/

gloost::Vector3
RandomSequence::crandVector()
{
  gloost::Vector3 vector;

  iteration();
  vector[0] = (_current/(mathType)_m)*2.0 - 1;
  iteration();
  vector[1] = (_current/(mathType)_m)*2.0 - 1;
  iteration();
  vector[2] = (_current/(mathType)_m)*2.0 - 1;

  return vector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief resets the sequence of random numbers
  \remarks ...
*/

void
RandomSequence::reset()
{
  _currents[0] = _seed;
  _currents[1] = 735883671;
  _currents[2] = 245234547;
  _currents[3] = 987654567;
  _current = _seed;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief performs one iteration
  \remarks ...
*/

void
RandomSequence::iteration()
{
  ++_iterationCounter;
  _currents[0] = (_a*_currents[0] + _c) % _m;
  _currents[1] = (_a*_currents[1] + _c) % _m;
  _currents[2] = (_a*_currents[2] + _c) % _m;
  _currents[3] = (_a*_currents[3] + _c) % _m;

  _current = (_currents[0]+_currents[1]+_currents[2]+_currents[3]) % _m;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
