
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



#ifndef H_GLOOST_TBO
#define H_GLOOST_TBO

// gloost includes
#include <gloost/gl/MultiGlContextEntity.h>

// cpp includes
#include <GL/glew.h>
#include <memory>


namespace gloost
{

  class BinaryBundle;


namespace gl
{

////////////////////////////////////////////////////////////////////////////////


  //  Texture Buffer Object

class Tbo : public MultiGlContextEntity
{
	public:

    // class constructor
    Tbo(const std::shared_ptr<BinaryBundle>& data,
        unsigned usage = GL_STATIC_DRAW);

    // class destructor
	  virtual ~Tbo();



	  // initialize the resource within a context
	  virtual bool initInContext (unsigned contextId = 0);

	  // remove a resource from a context
	  virtual void removeFromContext(unsigned contextId = 0);


    // bind the buffer into state
    virtual void bind(unsigned contextId = 0);

    // unbinds this buffer from state
	  static void unbind();


    // return the buffers opengl handle for a context
    unsigned getHandle(unsigned contextId = 0);


	protected:

    std::shared_ptr<BinaryBundle> _data;
    unsigned                      _usage;

};


} // namespace gl
} // namespace gloost


#endif // H_GLOOST_TBO





