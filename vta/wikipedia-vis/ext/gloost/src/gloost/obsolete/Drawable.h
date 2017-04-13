
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



#ifndef H_GLOOST_DRAWABLE
#define H_GLOOST_DRAWABLE


// cpp includes
#include <string>
#include <map>

// gloost includes
//#include <gloost/Vector3.h>
#include <gloost/gl/MultiGlContextEntity.h>

/// components of a drawable to be triggered on Drawable::bind(unsigned int components, unsigned int contextId)
//#define GLOOST_DRAWABLE_ATTRIBUTE_NORMALS   2
//#define GLOOST_DRAWABLE_ATTRIBUTE_TEXCOORDS 4

#define GLOOST_DRAWABLE_SUGGESTED_DRAWMODE 2368929


namespace gloost
{

  class Mesh;


namespace obsolete
{


  //  Base class of all drawables like Vbo or DisplayList

class Drawable : public gl::MultiGlContextEntity
{
	public:

    /// class constructor
    Drawable(unsigned int numResourcesPerContext, std::shared_ptr<Mesh> mesh);


    /// class destructor
	  virtual ~Drawable();


	  // increments reference counter
	  virtual Drawable* takeReference();

	  // decrements reference counter
	  virtual void dropReference();


    // bind the drawable into state by specify its attributes
    virtual void bind(unsigned int attributes, unsigned int contextId) = 0;

    // remove this drawable from state
	  virtual void disable(unsigned int contextId) = 0;



	  // draws the VBO with bound components
	  virtual void draw(unsigned contextId = 0u) = 0;


    // sets the draw mode of this Drawable. (GL_TRIANGLES, GL_POINTS, ...) Default is GL_TRIANGLES
    virtual void         setDrawMode(unsigned int drawMode);

    // returns the draw mode of this Drawable
    virtual unsigned int getDrawMode() const;

    // returns the draw mode suggested by analysing the mesh of this Drawable
    virtual unsigned int getSuggestedDrawMode() const;



    /// returns the Geometry of this Drawable
    std::shared_ptr<Mesh> getMesh();


	protected:

    std::shared_ptr<Mesh> _mesh;
    unsigned int          _drawMode;
    unsigned int          _suggestedDrawMode;


};

} // namespace obsolete
} // namespace gloost


#endif // H_GLOOST_DRAWABLE




