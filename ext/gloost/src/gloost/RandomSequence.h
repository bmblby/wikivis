
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



#ifndef H_GLOOST_RANDOMSEQUENCE
#define H_GLOOST_RANDOMSEQUENCE


// gloost system includes
#include <gloost/Vector3.h>
#include <gloost/gloostConfig.h>


// c++ includes
#include <iostream>
#include <vector>

namespace gloost
{

  // Generator, generates a sequence of "random" numbers with given seed

class RandomSequence
{

  public:
    RandomSequence();
    RandomSequence(unsigned seed);
    RandomSequence(unsigned seed, unsigned max);

    // returns a random number between 0.0 ... 1.0
    mathType frand();

    // returns a random number between -1.0 ... 1.0
    mathType crand();

    // returns true if a random value between 0.0 and 1.0 is smaller as <probability>
    bool chance(float probability = 0.5);

    // returns a random vector with components between 0.0 ... 1.0
    Vector3 frandVector();

    // returns a random vector with components between -1.0 ... 1.0
    Vector3 crandVector();


    // resets the sequence of random numbers
    void reset();

  protected:

    // performs one iteration
    void iteration();

    unsigned _seed;
    std::vector<unsigned> _currents;
    unsigned _current;
    unsigned _iterationCounter;
    unsigned _a;
    unsigned _c;
    unsigned _m;


};

} // namespace gloost

#endif // H_GLOOST_RANDOMSEQUENCE
