
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

#ifndef H_GLOOST_HUMAN_INPUT_ARCBALL
#define H_GLOOST_HUMAN_INPUT_ARCBALL

#include <gloost/Vector3.h>
#include <gloost/Matrix.h>
#include <memory>


namespace gloost
{
namespace human_input
{

 // Transforms mouse movement to an ArcBall

class ArcBall
{
  public:

    typedef std::shared_ptr<ArcBall>       shared_ptr;
    typedef std::shared_ptr<const ArcBall> const_shared_ptr;

    static shared_ptr create(float width, float height);

    //Create/Destroy
    virtual ~ArcBall();

    //Set new bounds
    void setScreenSize(float width, float height);

    // returns the horizontal length of the interation area
    float getScreenSizeX() const;
    // returns the vertical length of the interation area
    float getScreenSizeY() const;

    // Mouse down befor drag(...)
    void click(gloost::Point3& point);

    // Mouse drag, calculate rotation and returns a quaternion as an  gloost::Vector3(x,y,z,w)
    gloost::Vector3 drag(const gloost::Point3& point);

    // returns a quaternion
    gloost::Vector3 getQuaternionForMouseMovement(const gloost::Point3& p1, const gloost::Point3& ) const;


    // converts a quaternion into a rotation matrix
    static gloost::Matrix getRotationFromFromQuaternion(const gloost::Vector3& quat);


  protected:

    ArcBall(float width, float height);


  private:

    gloost::Vector3 mapToSphere(const gloost::Point3& point) const;

    /// Saved click vector
    gloost::Vector3 _startVector;

    /// Saved drag vector
    gloost::Vector3 _endVector;

    /// size of the arcball in screen coords
    float     _screenSizeNormalizerX;

    /// size of the arcball in screen coords
    float     _screenSizeNormalizerY;

};

} // namespace human_input
} // namespace gloost

#endif // H_GLOOST_HUMAN_INPUT_ARCBALL

