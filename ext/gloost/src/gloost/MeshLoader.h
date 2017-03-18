
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



#ifndef H_GLOOST_MESHLOADER
#define H_GLOOST_MESHLOADER


// gloost includes
#include <gloost/gloostConfig.h>

// cpp includes
#include <string>
#include <vector>
#include <memory>



namespace gloost
{

  class Mesh;


//////////////////////////////////////////////////////////////////////////////////////////


  ///  Base class of all mesh loader

class MeshLoader
{
	public:

    // class constructor
    MeshLoader(const std::string& filePath);
    MeshLoader(const gloost::PathType& filePath);

    // class destructor
	  virtual ~MeshLoader();


    // returns a pointer to the mesh
	  virtual std::shared_ptr<Mesh> getMesh();

    // returns the file path of the mesh file
	  const std::string getFilePathString() const;

	  const gloost::PathType& getFilePath() const;


	protected:

    // read the file
    virtual bool readFile(const std::string& filePath) = 0;

//    virtual bool readFile(const gloost::PathType& filePath) = 0;

    gloost::PathType _filePath;
    std::shared_ptr<Mesh> _mesh;
};


} // namespace gloost


#endif // H_GLOOST_MESHLOADER

