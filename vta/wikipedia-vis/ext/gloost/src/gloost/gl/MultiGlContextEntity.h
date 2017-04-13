
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


#ifndef GLOOST_MULTI_GL_CONTEXT_ENTITY_H
#define GLOOST_MULTI_GL_CONTEXT_ENTITY_H



// gloost system includes
#include <gloost/gloostMath.h>
#include <gloost/SharedResource.h>


// cpp includes
#include <string>
#include <vector>



namespace gloost
{
namespace gl
{


  // Base class of all GL Resources like Shader or Texture which can live in up to 32 GL contexts

class MultiGlContextEntity : public SharedResource
{
	public:

    // class constructor
    MultiGlContextEntity(unsigned numResourcesPerContext = 1u);

    // class destructor
	  virtual ~MultiGlContextEntity();



	  // initialize the resource within a context
	  virtual bool initInContext (unsigned contextId = 0u) = 0;


	  // remove a resource from a context
	  virtual void removeFromContext(unsigned contextId = 0u) = 0;



    // set a handle for a context and a resource Id
    void setResourceHandleForContext(unsigned handle,
                                     unsigned contextId,
                                     unsigned resourceId = 0);


    // set a handle for a context and a resource Id
    unsigned& getResourceHandleForContext( unsigned contextId,
                                           unsigned resourceId = 0u);

    // set a handle for a context and a resource Id
    const unsigned& getResourceHandleForContext( unsigned contextId,
                                                 unsigned resourceId = 0u) const;

    // get the number of contexts this object lives in
    unsigned getNumContexts() const;

    // get the number of resources per context
    unsigned getNumResourcesPerContext() const;

    // returns true if the handle exists within the context contextId
    bool handleInContextExists(unsigned contextId, unsigned resourceId = 0u) const;

	protected:

    /// contextId <-> GL Handle, of the resource within this GL Context
    std::vector<std::vector<unsigned> > _handlesVectorForContexts;

    /// number of resources per context of this instance
    unsigned _numResourcesPerContext;

};


} // namespace gl
} // namespace gloost


#endif // GLOOST_MULTI_GL_CONTEXT_ENTITY_H


