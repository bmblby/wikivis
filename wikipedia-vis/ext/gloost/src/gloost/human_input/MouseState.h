
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



#ifndef H_GLOOST_HUMAN_INPUT_MOUSESTATE
#define H_GLOOST_HUMAN_INPUT_MOUSESTATE


#define GLOOST_MOUSESTATE_BUTTON0 0u
#define GLOOST_MOUSESTATE_BUTTON1 1u
#define GLOOST_MOUSESTATE_BUTTON2 2u
#define GLOOST_MOUSESTATE_BUTTON3 3u
#define GLOOST_MOUSESTATE_BUTTON4 4u
#define GLOOST_MOUSESTATE_BUTTON5 5u
#define GLOOST_MOUSESTATE_BUTTON6 6u
#define GLOOST_MOUSESTATE_NUM_BUTTONS 7u

#define GLOOST_MOUSESTATE_WHEELUP   7u
#define GLOOST_MOUSESTATE_WHEELDOWN 8u



// gloost system includes
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/Matrix.h>


// cpp includes
#include <string>
#include <memory>
#include <map>


namespace gloost
{

namespace human_input
{


  //  stores the state of the mouse

class MouseState
{

	public:

    // a std::shared_ptr of a MouseState instance
    typedef std::shared_ptr<MouseState>       shared_ptr;
    typedef std::shared_ptr<const MouseState> const_shared_ptr;

    typedef std::vector<bool> ButtonStateContainer;

    // class factory
    static shared_ptr create();
    static shared_ptr create(shared_ptr mouseState);

    // class constructor
    MouseState();

    // class destructor
    virtual ~MouseState();

    /// get the location of the mouse
    const Point3& getPosition() const;
    Point3& getPosition();
    void   setPosition(const Point3& location);
    void   setPosition(float x, float y, float z = 0.0);

    /// get the location of the mouse one frame before
    const Point3& getOldPosition() const;

    /// get the coordinates of the last mouseUp
    const Point3& getLastMouseDownPosition() const;

    /// get the coordinates of the last mouseUp
    const Point3& getLastMouseUpPosition() const;

    /// get the moving speed of the mouse (in pixel/frame)
    Vector3& getSpeed();
    const Vector3& getSpeed() const;

    /// set the speed of the mouse to zero
    void setSpeedToZero();

    /// reset mouse events so they can be set in the next frame
    void resetMouseEvents();

    /// get the state of one of the mouse bottons (GLOOST_MOUSESTATE_BUTTON1, GLOOST_MOUSESTATE_BUTTON2, GLOOST_MOUSESTATE_BUTTON3, ...)
    bool getButtonState(unsigned buttonNum) const;
    void setButtonState(unsigned buttonNum, bool state);

    bool getButtonDownEvent(unsigned buttonNum) const;
    void setButtonDownEvent(unsigned buttonNum, bool eventToggle);

    bool getButtonUpEvent(unsigned buttonNum) const;
    void setButtonUpEvent(unsigned buttonNum, bool eventToggle);

    // transforms the mouse position and speed
    void transform(const gloost::Matrix& transformation);

	private:

    Point3               _position;
    Point3               _oldPosition;
    Vector3              _speed;
    Point3               _lastDownPos;
    Point3               _lastUpPos;
    ButtonStateContainer _buttonStates;
    ButtonStateContainer _mouseDownEvents;
    ButtonStateContainer _mouseUpEvents;
};

} // namespace human_input
} // namespace gloost

#endif // H_GLOOST_HUMAN_INPUT_MOUSESTATE
