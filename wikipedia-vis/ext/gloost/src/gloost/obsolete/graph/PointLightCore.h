
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



#ifndef GLOOST_POINTLIGHTCORE_H
#define GLOOST_POINTLIGHTCORE_H


// gloost system includes
#include <gloost/graph/Core.h>
#include <gloost/graph/Node.h>

#include <gloost/PointLight.h>


// cpp includes
//#include <GL/glew.h>
//#include <GL/gl.h>


namespace gloost
{
namespace graph
{



class Action;


 /// This core holds a PointLight

class PointLightCore : public Core
{
	public:
		 PointLightCore (GLenum glLightNum = GL_LIGHT0);
		 PointLightCore (PointLight* pointLight, GLenum glLightNum = GL_LIGHT0);
		~PointLightCore ();



		/* virtual */ void enterCore (const Action* action);
		/* virtual */ void exitCore  (const Action* action);



    /// get the geometry
		PointLight* getPointLight() const;



		/// get the light beacon node (node that determines light position)
		Node* getBeacon() const;

		/// set the light beacon node (node that determines light position)
		void setBeacon(Node* beaconNode);



		/// get glLight number for this light
		GLenum getGlLightNum() const;

		/// set glLight number for this light
		void setGlLightNum(GLenum glLightNum);



  private:

    GLenum      _glLightNum;
    PointLight* _pointLight;
    Node*       _beacon;

    /// visualize the light with a sphere of size _size
    float _visualSize;

};


} // namespace graph
} // namespace gloost

#endif // GLOOST_POINTLIGHTCORE_H
