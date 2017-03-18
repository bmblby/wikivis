
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

#ifndef H_GLOOST_VBO
#define H_GLOOST_VBO

// cpp includes
#include <string>
#include <vector>
#include <GL/glew.h>
#include <map>
#include <memory>

// gloost includes
#include <gloost/gl/MultiGlContextEntity.h>

#define GLOOST_VBO_RESOURCE_VAO 0 // vertex array object
#define GLOOST_VBO_RESOURCE_VBO 1  // vertex buffer object

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif

//#define DEBUG_COUNT_INSTANCES
#ifdef DEBUG_COUNT_INSTANCES
  #include <atomic>
#endif


namespace gloost
{

  class InterleavedAttributes;

namespace gl
{

////////////////////////////////////////////////////////////////////////////////


  //  Vertex Buffer Object with an Vertex Array Object stroring its layout

class Vbo : public MultiGlContextEntity
{
	public:

    typedef std::shared_ptr<Vbo>       shared_ptr;
    typedef std::shared_ptr<const Vbo> const_shared_ptr;

    // class factory
    static shared_ptr create (const std::shared_ptr<const gloost::InterleavedAttributes>& vertexAttribs,
                              unsigned usageMode = GL_STATIC_DRAW);

    // class constructor
    Vbo(const std::shared_ptr<const gloost::InterleavedAttributes>& vertexAttribs,
        unsigned usageMode = GL_STATIC_DRAW);

    // class destructor
	  virtual ~Vbo();


    // bind the drawable into state by specify its attributes
    virtual void bind(unsigned contextId = 0u);

    // remove this drawable from state
	  virtual void unbind(unsigned contextId = 0u);


	  // draws all vertices via glDrawArrays
	  virtual void draw(unsigned drawMode);

	  // draws count vertices via glDrawArrays starting from startIndex
	  virtual void draw(unsigned drawMode, unsigned startIndex, unsigned count);


	  // initialize the resource within a context
	  virtual bool initInContext (unsigned contextId = 0);

	  // remove a resource from a context
	  virtual void removeFromContext(unsigned contextId = 0);


    // returns the VertexAttribContainer used by this Vbo
    const std::shared_ptr<const InterleavedAttributes>& getVertexAttributes() const;

    // sets the InterleavedAttributes as data for this Vbo instance
    void setVertexAttributes(const std::shared_ptr<InterleavedAttributes>& attributes);


    // returns the GL handle of the VAO
    unsigned getVaoHandle(unsigned contextId = 0);

    // returns the GL handle of the VBO
    unsigned getVboHandle(unsigned contextId = 0);


	protected:

	  std::shared_ptr<const InterleavedAttributes> _vertexAttributes;

	  unsigned _usageMode;

  private:

#ifdef DEBUG_COUNT_INSTANCES
   static std::atomic<int> _instanceCounter;
#endif
};

} // namespace gl
} // namespace gloost

#endif // H_GLOOST_VBO
