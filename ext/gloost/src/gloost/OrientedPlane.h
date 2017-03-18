
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



#ifndef H_GLOOST_ORIENTEDPLANE
#define H_GLOOST_ORIENTEDPLANE



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/Plane.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{

  //  A plane consisting of a point, a normal and a rotation around the normal

class OrientedPlane: public Plane
{
	public:

    /// a std::shared_ptr of an OrientedPlane instance
    typedef std::shared_ptr<OrientedPlane>       shared_ptr;
    typedef std::shared_ptr<const OrientedPlane> const_shared_ptr;

    static const gloost::Vector3 referenceTangent;

    // class constructor
    OrientedPlane();

    // class factoy
    static shared_ptr create();

    // class destructor
	  virtual ~OrientedPlane();

	  Vector3 getTangent() const;
	  Vector3 getBinormal() const;

    // returns the rotation of the plane around the normal
	  mathType getRotation() const;
	  void setRotation(const mathType& rotation);

    // inits the OrientedPlane
    virtual void transform(const gloost::Matrix& matrix, bool normalizeNormal = true);

	protected:


	private:

    mathType _rotation;


};


} // namespace gloost


#endif // H_GLOOST_ORIENTEDPLANE


