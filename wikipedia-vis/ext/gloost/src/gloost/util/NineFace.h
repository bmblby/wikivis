
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



#ifndef H_GLOOST_UTIL_NINEFACE
#define H_GLOOST_UTIL_NINEFACE



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/Vector3.h>


// cpp includes
#include <string>
#include <memory>
#include <vector>



namespace gloost
{


  class Mesh; class InterleavedAttributes;



namespace util
{


  //  creates and stores a parameters and a generated mesh for a nineface

class NineFace
{
	public:

    /// a std::shared_ptr of an NineFace instance
    typedef std::shared_ptr<NineFace>       shared_ptr;
    typedef std::shared_ptr<const NineFace> const_shared_ptr;


    // class factoy
    static shared_ptr create(unsigned textureWidth,
                             float left, float right,
                             float bottom, float top,
                             bool alignVertexPosToInt = false);



    // class destructor
	  virtual ~NineFace();



    // returns the inner size of the nineface
    gloost::Vector3 getInnerSize() const;

    // sets the the inner size of the nineface
    void setInnerSize(const gloost::Vector3& size);



    // returns the inner size of the nineface
    const gloost::Vector3& getOuterSize() const;

    // sets the the inner size of the nineface
    void setOuterSize(const gloost::Vector3& size);



    // returns a vector {left, right, bottom, top}
    std::vector<float> getLRBT() const;



    // returns true if the mesh is not up to date
    bool isMeshDirty() const;

    // setMeshDirty
    void setMeshDirty(bool flag = true);



    // inits the NineFace
	  std::shared_ptr<Mesh> getMesh();
//
//    // inits the NineFace
//	  std::shared_ptr<InterleavedAttributes> getVertexAttribs();


	protected:

    unsigned _textureWidth;
    Vector3     _outerSize;
    float _left; float _right;

    float _bottom; float _top;
    bool  _alignVertexPosToInt;

    gloost::gloostId _textureGid;

    bool _meshHasChanged;

    std::shared_ptr<Mesh>                  _mesh;
    std::shared_ptr<InterleavedAttributes> _vertexAttributes;



    // class constructor
    NineFace();


    // generates the mesh
    void generate();



	private:

   // ...

};


} // namespace util
} // namespace gloost


#endif // H_GLOOST_UTIL_NINEFACE


