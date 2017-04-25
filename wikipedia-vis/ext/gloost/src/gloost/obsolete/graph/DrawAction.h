
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



#ifndef GLOOST_DRAWACTION_H
#define GLOOST_DRAWACTION_H



// gloost system includes
#include <gloost/graph/Node.h>
#include <gloost/graph/Action.h>
#include <gloost/Camera.h>


// cpp includes
#include <iostream>


namespace gloost
{
namespace graph
{



 /// Standart Draw Action


class DrawAction: public Action
{

	public:
		/// class constructor
		DrawAction();

		/// class constructor
		DrawAction(int contextId);


		/// class destructor
		~DrawAction();


    /// Run and return last Node visited
    /* virtual */ void start (Node* startNode, Camera* camera);


    /// returns the current camera
    Camera* getCurrentCamera();



	private:

    void draw (Node *myNode);

		Camera* _currentCamera;

};


} // namespace graph
} // namespace gloost


#endif // GLOOST_DRAWACTION_H
