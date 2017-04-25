
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
#include <gloost/gl/MultiGlContextEntity.h>
#include <gloost/gloostConfig.h>
//#include <MessageLog.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace gl
{

/**
  \class MultiGlContextEntity

  \author Felix Weiszig
  \date   December 2009

  \brief Base class of all GL Resources like Shader or Texture which can live
  in up to 32 GL contexts. The methodes initInContext() and removeFromContext()
  must be implemented in your inherited classes

  \remarks You have to specify the number of contexts your gloost lib can handle
  by setting the GLOOST_SYSTEM_NUM_RENDER_CONTEXTS constant within the gloostConfig.h.
  If you only use one context (wich is the normal/default case) you can set this value to 1
  to gain some speed.
*/


////////////////////////////////////////////////////////////////////////////////


/**

  \brief Class constructor
  \param numResourcesPerContext Number of resources per context your class shall handle.
*/

MultiGlContextEntity::MultiGlContextEntity(unsigned numResourcesPerContext):
  SharedResource(),
  _handlesVectorForContexts(GLOOST_SYSTEM_NUM_RENDER_CONTEXTS,
                            std::vector<unsigned>(numResourcesPerContext, 0u)),
  _numResourcesPerContext(numResourcesPerContext)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
*/

MultiGlContextEntity::~MultiGlContextEntity()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Sets a resource handle for a resource within a context
  \param handle The handle (normaly the OpenGL handle) of the resource
  \param contextId The id of the context this handle shall be associated with,
         0 or none if you have only one context
  \param resourceId The id of the resource, 0 or none if you have only one resource per context
*/


void
MultiGlContextEntity::setResourceHandleForContext( unsigned handle,
                                             unsigned contextId,
                                             unsigned resourceId)
{
  _handlesVectorForContexts[contextId][resourceId] = handle;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the handle for a specified resource of a context
  \param contextId The id of the context the resource was generated for,
        0 or none if you have only one context
  \param resourceId The id of the resource, 0 or none if you have only
         one resource per context
*/

unsigned&
MultiGlContextEntity::getResourceHandleForContext(unsigned contextId,
                                            unsigned resourceId)
{
  return _handlesVectorForContexts[contextId][resourceId];
}


////////////////////////////////////////////////////////////////////////////////


/**

  \brief Returns the handle for a specified resource of a context
  \param contextId The id of the context the resource was generated for,
        0 or none if you have only one context
  \param resourceId The id of the resource, 0 or none if you have only
         one resource per context

*/


const unsigned&
MultiGlContextEntity::getResourceHandleForContext(unsigned contextId,
                                            unsigned resourceId) const
{
  return _handlesVectorForContexts[contextId][resourceId];
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the number of contexts this object lives in. This counts only
         contexts your resource was allready generated for.

  \remarks 1. This method is not very fast due to two nested loops.
  \remarks 2. The maximal number of contexts supported by your gloost lib is
           defined in the GLOOST_SYSTEM_NUM_RENDER_CONTEXTS constant within the
           gloostConfig.h.
*/

unsigned
MultiGlContextEntity::getNumContexts() const
{
  unsigned count = 0u;

  for (unsigned i=0u; i!=_handlesVectorForContexts.size(); ++i)
  {
    for (unsigned j=0u; j!=_numResourcesPerContext; ++j)
    {
      if (_handlesVectorForContexts[i][j])
      {
        ++count;
        break;
      }
    }
  }

  return count;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the number of resources per context this instance can have.
*/

unsigned
MultiGlContextEntity::getNumResourcesPerContext() const
{
  return _numResourcesPerContext;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns true if a handle exists within the context contextId
  \param contextId The id of the context, 0 or none if you have only one context
  \param resourceId The id of the resource, 0 or none if you have only
         one resource per context
*/

bool
MultiGlContextEntity::handleInContextExists(unsigned contextId, unsigned resourceId) const
{
  return (bool) _handlesVectorForContexts[contextId][resourceId];
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gl

} // namespace gloost


