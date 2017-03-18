
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



#include <gloost/graph/Collector.h>
#include <gloost/Mesh.h>



namespace gloost
{
namespace graph
{


 // class constructor

Collector::Collector():
    _triangleCount(0),
    _vertexCount(0),
    _traversTime(0),
    _nodesCount(0),
    _coreCount(),
    _coreTimes()
{


}


///////////////////////////////////////////////////////////////////////////////


 // class destructor

Collector::~Collector()
{


}


///////////////////////////////////////////////////////////////////////////////


 /// Start collecting

void
Collector::startCollecting()
{


}


///////////////////////////////////////////////////////////////////////////////


 /// end collecting

void
Collector::endCollecting()
{


}


///////////////////////////////////////////////////////////////////////////////


 /// Reset Collector

void
Collector::reset()
{

  _triangleCount = 0;
  _vertexCount   = 0;
  _traversTime   = 0;
  _nodesCount    = 0;

  _coreCount.clear();

  /// Holds rendertime for each core
  _coreTimes.clear();
}


///////////////////////////////////////////////////////////////////////////////


 /// Start messuring a core

void
Collector::beginCore(Core* core)
{
  // count nodes visited
  _nodesCount++;

  // count specific core type
  _coreCount[core->getType()]++;


  if (core->getType() == GLOOST_CORE_VBO)
  {
    VboCore* vboCore = dynamic_cast<VboCore*>(core);

//    if (castedCore->getGeometry())
    {
      _triangleCount += vboCore->getVbo()->getMesh()->getTriangles().size();
      _vertexCount   += vboCore->getVbo()->getMesh()->getPositions().size();
    }
  }
  // else if () ...

}


///////////////////////////////////////////////////////////////////////////////


 /// End messuring core

void
Collector::endCore(Core* core)
{


}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Returs number of points

unsigned int
Collector::getVertexCount() const
{
  return _vertexCount;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// Returns number of Triangles

unsigned int
Collector::getTriangleCount() const
{
  return _triangleCount;
}


///////////////////////////////////////////////////////////////////////////////


 /// Returns number of Triangles

unsigned int
Collector::getNodeCount() const
{
  return _nodesCount;
}


///////////////////////////////////////////////////////////////////////////////


  /// get number of cores for a type

unsigned int
Collector::getCoreCount()
{

  unsigned int num = 0;

  for(unsigned int i=0; i != _coreCount.size(); ++i)
  {
    num += _coreCount[i];
  }


  return num;
}


///////////////////////////////////////////////////////////////////////////////


  /// get number of cores for a type

unsigned int
Collector::getCoreCount(unsigned int coreTypeId)
{
  return _coreCount[coreTypeId];
}


///////////////////////////////////////////////////////////////////////////////


} // namespace graph
} // namespace gloost
