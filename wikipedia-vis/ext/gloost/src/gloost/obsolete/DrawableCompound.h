
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



#ifndef H_GLOOST_DRAWABLECOMPOUND
#define H_GLOOST_DRAWABLECOMPOUND



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{

  class Mesh;


namespace obsolete
{

  class Vbo;



  //  Compound containing a Mesh and a Vbo to use them together

class DrawableCompound
{
	public:

    // class constructor
    DrawableCompound();
    // class constructor
    DrawableCompound(std::shared_ptr<Mesh> mesh);

    // class destructor
	  virtual ~DrawableCompound();


    // sets the Mesh dirty to indicate a change within its content and trigger an Vbo update
	  void setMeshDirty(bool value = true);

    // updates the vbo, necessary if Mesh was changed
	  void updateVbo();


	  // clears the Mesh and removes resources from GPU
	  void clear();


    // replaces the current Mesh with another one
	  void setMesh(std::shared_ptr<Mesh> mesh);

    // returns the Mesh
	  std::shared_ptr<Mesh> getMesh();

    // returns the Vbo
	  Vbo* getVbo();


	  // draws the compound
	  void draw();


	  // draws the compound, but updates the Vbo befor if necessary
	  void updateAndDraw();


	protected:


    std::shared_ptr<Mesh> _mesh;
    Vbo*  _vbo;


    bool _meshDirty;


};


} // namespace obsolete
} // namespace gloost


#endif // H_GLOOST_DRAWABLECOMPOUND


