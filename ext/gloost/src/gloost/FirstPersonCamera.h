
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



#ifndef H_GLOOST_FIRST_PERSON_CAMERA
#define H_GLOOST_FIRST_PERSON_CAMERA



// gloost includes
#include <gloost/PerspectiveCamera.h>
#include <gloost/Ray.h>



namespace gloost
{


  //  A PerspectiveCamera with an interface supporting first person view

  class FirstPersonCamera : public PerspectiveCamera
  {

  public:


    // class constructor
    FirstPersonCamera();
//    FirstPersonCamera(float fov, float aspect, float near, float far);

    // class destructor
    virtual ~FirstPersonCamera();

    // rotates the viewer around itself
//    void setRotationH(mathType h);


    // sets the position of the first person camera
    void setPosition(const gloost::Point3 position);

    // moves the camera
    void move (const Vector3& speed);


//    // sets
//    void setFriction(mathType friction);
//
//    // frame step


    // rotate viewer horizontally
    void rotateH (mathType speed);

    // rotate viewer vertically
    void rotateV (mathType speed);

    // sets the up vector
    void setUpVector(const gloost::Vector3& upVector);


    // applies the modelview and projection to the GL state
//    virtual void set();



//    /* virtual */ void set();

  protected:

    Point3   _position;
    mathType _friction;

    Point3   _rotation;

    Point3   _lookAt;


    gloost::Vector3 _upVector;

    bool     _dirtyLookAt;



};


} // namespace gloost


#endif // H_GLOOST_FIRST_PERSON_CAMERA
