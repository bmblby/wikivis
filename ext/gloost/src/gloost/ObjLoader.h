
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



#ifndef H_GLOOST_OBJLOADER
#define H_GLOOST_OBJLOADER


/// gloost includes
#include <gloost/Vector3.h>
#include <gloost/Point3.h>
#include <gloost/Ray.h>
#include <gloost/MeshLoader.h>
#include <gloost/ObjMatFile.h>


/// cpp includes
#include <string>
#include <vector>



namespace gloost
{

  class Geometry;


//////////////////////////////////////////////////////////////////////////////////////////


  ///  Loads the horrible Wavefront *.mtl file and provides all materials as UniformSet(s)

class ObjLoader : public MeshLoader
{
	public:
    /// class constructor
    ObjLoader(const std::string& filePath);

    /// class destructor
	  virtual ~ObjLoader();

    /// returns a Mesh with the loaded data
	  std::shared_ptr<Mesh> getMesh();

    /// ...
	  ObjMatFile& getMaterials();

	protected:


    /// reads an *.obj file
    bool readFile(const std::string& filePath);

    Point3 readFaceString(std::ifstream& inStream);

    ObjMatFile _materials;

};


} // namespace gloost


#endif // H_GLOOST_OBJLOADER

