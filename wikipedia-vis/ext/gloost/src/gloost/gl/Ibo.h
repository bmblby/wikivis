
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

#ifndef H_GLOOST_IBO
#define H_GLOOST_IBO

// gloost includes
#include <gloost/gl/MultiGlContextEntity.h>

// cpp includes
#include <string>
#include <vector>
#include <map>

#define GLOOST_IBO_SUGGESTED_DRAWMODE 2368929


namespace gloost
{

  class Mesh;


namespace gl
{


////////////////////////////////////////////////////////////////////////////////


  //  Index Buffer Object to use with a gloost::gl::Vbo

class Ibo : public MultiGlContextEntity
{
	public:

    /// std::shared_ptr
    typedef std::shared_ptr<Ibo>       shared_ptr;
    typedef std::shared_ptr<const Ibo> const_shared_ptr;

    // class factory
    static shared_ptr create (const std::shared_ptr<Mesh>& mesh,
                              unsigned drawMode = GLOOST_IBO_SUGGESTED_DRAWMODE);

    // class constructor
    Ibo(const std::shared_ptr<Mesh>& mesh, unsigned drawMode = GLOOST_IBO_SUGGESTED_DRAWMODE);
    Ibo(const std::shared_ptr<const Mesh>& mesh, unsigned drawMode = GLOOST_IBO_SUGGESTED_DRAWMODE);

    // class constructor
    Ibo(const std::vector<unsigned>& indices, unsigned drawMode = GLOOST_IBO_SUGGESTED_DRAWMODE);


    // class destructor
	  virtual ~Ibo();


    // bind the drawable into state by specify its attributes
    virtual void bind(unsigned contextId = 0);

    // remove this drawable from state
	  virtual void unbind();


	  // draws the ibo
	  /*virtual*/ void draw();

	  // draws the ibo for a custom drawMode
	  /*virtual*/ void draw(unsigned drawMode);

//	   void draw(int startIndex, int endIndex, unsigned contextId = 0);

    // sets the draw mode
    void setDrawMode(unsigned drawMode);

    // returns the draw mode
    unsigned getDrawMode();

    // returns the number of indices
    const unsigned& getNumIndices() const;


	  // initialize the resource within a context
	  virtual bool initInContext (unsigned contextId = 0);

	  // remove a resource from a context
	  virtual void removeFromContext(unsigned contextId = 0);


    // returns the Mesh
    const std::shared_ptr<const Mesh>& getMesh() const;


	protected:

	  std::shared_ptr<const Mesh> _mesh;

	  unsigned _drawMode;
	  unsigned _suggestedDrawMode;
	  unsigned _numIdices;
};


} // namespace gl
} // namespace gloost


#endif // H_GLOOST_IBO





