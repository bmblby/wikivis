
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

// gloost includes
#include <gloost/obsolete/Drawable.h>
#include <gloost/Vector3.h>
#include <gloost/Point3.h>
#include <gloost/Material.h>
#include <gloost/BitMask.h>
#include <gloost/Mesh.h>

#define GLOOST_VBO_RESOURCE_ELEMENT_ARRAY       0
#define GLOOST_VBO_RESOURCE_INTERLEAVED_ARRAY   1
#define GLOOST_VBO_NUM_RESOURCES                2

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif

namespace gloost
{

namespace obsolete
{

struct VboIndexRange
{
  VboIndexRange()
  {
    _material   = 0;
    _startIndex = 0;
    _length     = 0;
  }

  VboIndexRange(Material* material, unsigned int startIndex, unsigned int length)
  {
    _material   = material;
    _startIndex = startIndex;
    _length     = length;
  }

  VboIndexRange(const VboIndexRange& vir)
  {
    _material   = vir._material;
    _startIndex = vir._startIndex;
    _length     = vir._length;
  }


  Material*    _material;
  unsigned int _startIndex;
  unsigned int _length;
};


////////////////////////////////////////////////////////////////////////////////


  //  Base class of all drawables like Vbo or DisplayList

class Vbo : public Drawable
{
	public:

    // class constructor
    Vbo(std::shared_ptr<Mesh> mesh);

    // class destructor
	  virtual ~Vbo();



	  // increments reference counter
	  virtual Vbo* takeReference();

	  // decrements reference counter
	  virtual void dropReference();



	  // initialize the resource within a context
	  /*virtual*/ bool initInContext (unsigned contextId = 0u);

	  // remove a resource from a context
	  /*virtual*/ void removeFromContext(unsigned contextId = 0u);



    // bind the drawable into state by specify its attributes
    /*virtual*/ void bind(unsigned int supportIndexRanges = 0,
                          unsigned contextId = 0u);

    // remove this drawable from state
	  /*virtual*/ void disable(unsigned contextId = 0u);



	  // draws the VBO with bound components
	  /*virtual*/ void draw(unsigned contextId = 0u);


	protected:

    unsigned int _elementArraySize;

    // indicates supported vertex attributes within the interleaved structure
    BitMask             _supportedInterleavedComponents;
    MeshInterleavedInfo _interleavedInfo;
};


} // namespace obsolete
} // namespace gloost


#endif // H_GLOOST_VBO





