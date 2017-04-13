
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



#ifndef H_GLOOST_PLYLOADER
#define H_GLOOST_PLYLOADER


// gloost includes
#include <gloost/SharedResource.h>
#include <gloost/MeshLoader.h>
#include <gloost/BinaryFile.h>
#include <gloost/Vector3.h>
#include <gloost/Point3.h>
#include <gloost/ObjMatFile.h>


// cpp includes
#include <string>
#include <vector>



namespace gloost
{

  class Geometry;


//////////////////////////////////////////////////////////////////////////////////////////


  ///  Loads Ply Mesh file format

class PlyLoader : public MeshLoader
{
	public:
    /// class constructor
    PlyLoader(const gloost::PathType& filePath);

    /// class destructor
	  virtual ~PlyLoader();

	  std::shared_ptr<Mesh> getMesh();


	protected:

    // Property types
    enum PLY_PROPERTY_TYPE
    {
      PLY_TYPE_INT8  = 0,   // 0
      PLY_TYPE_INT16,       // 1
      PLY_TYPE_INT32,       // 2
      PLY_TYPE_FLOAT32,     // 3
      PLY_TYPE_FLOAT64,     // 4
      PLY_TYPE_LIST,        // 5
      PLY_TYPE_UNKNOWN      // 6
    };

    /// A property/component of an Ply element
    struct PlyProperty
    {
      std::string       name;
      PLY_PROPERTY_TYPE type;

      union
      {
        unsigned char  Int8;
        unsigned short Int16;
        unsigned int   Int32;
        float          Float32;
        double         Double;
        struct PlyListProperty
        {
          PLY_PROPERTY_TYPE countType;
          PLY_PROPERTY_TYPE itemType;
        } list;
      } data;


    };

    /// Ply element
    struct PlyElement
    {
      /// Name of the element like "vertex", "face"
      std::string name;
      /// Number of elements
      unsigned int count;
      // Properties of this element
      std::vector<PlyProperty> properties;
    };





    // Reads an *.ply file
    bool readFile(const std::string& filePath);

    // Reads an element line of the header
    void readHeaderElement();

    // Reads a property of the header and adds it to the current element
    void readHeaderProperty();

    // Reads the vertices
    void readVertices(const PlyElement& element);

    // Reads the faces
    void readFaces(const PlyElement& element);

    // Reads a numeric property value
    float readPropertyValue(PLY_PROPERTY_TYPE propertyType);

    // Interpretes a ply type string to internal type
    PLY_PROPERTY_TYPE getPropertyType(const char* typeString) const;





    /// file path of the ply file
    std::string _filePath;

    /// file to read from
    BinaryFile _inFile;

    /// vector of ply elements to parse
    std::vector<PlyElement> _plyElements;

    /// Ply document info
    struct PlyFileInfo
    {
      std::string formatString;
      float       version;
      std::vector<std::string> comments;
    } _fileInfo;


};


} // namespace gloost


#endif // H_GLOOST_PLYLOADER


