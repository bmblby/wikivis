
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



#ifndef GLOOST_COLLECTOR_H
#define GLOOST_COLLECTOR_H


// gloost system includes
#include <gloost/graph/Core.h>
#include <gloost/graph/VboCore.h>

// cpp includes
#include <map>


namespace gloost
{
namespace graph
{


  ///  Attached to an Action this class collects statistic info of visited nodes like their count or the number of cores of each type

class Collector
{
	public:
    // class constructor
    Collector();
    // class destructor
    virtual ~Collector();


    /// Start collecting
    virtual void startCollecting();

    /// end collecting
    virtual void endCollecting();

    /// Reset Collector
    virtual void reset();


    /// Start messuring a core
    virtual void beginCore(Core* core);

    /// End messuring core
    virtual void endCore(Core* core);


    /// Get Properties
    unsigned int getTriangleCount () const;
    unsigned int getVertexCount () const;
    unsigned int getNodeCount() const;
    unsigned int getCoreCount();
    unsigned int getCoreCount(unsigned int coreTypeId);



	protected:

    int   _triangleCount;
    int   _vertexCount;
    float _traversTime;
    int   _nodesCount;

    std::map<unsigned int, int> _coreCount;

    /// Holds rendertime for each core
    std::map<Core*, float> _coreTimes;


};


} // namespace graph
} // namespace gloost

#endif // GLOOST_COLLECTOR_H
