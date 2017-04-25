
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

#ifndef H_GLOOST_HUMANINPUT_KeyboardState
#define H_GLOOST_HUMANINPUT_KeyboardState

// gloost system includes
#include <gloost/human_input/GenericSignalMapper.h>
//#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <memory>

namespace gloost
{
namespace human_input
{

  /// defines all supported KEYBOARD EVENTS
  enum KeyEvent
  {
    CTRL_LEFT = 10000u, CTRL_RIGHT,
    ALT_LEFT, ALT_RIGHT,
    SHIFT_LEFT, SHIFT_RIGHT,
    ENTER, ENTER_KEYPAD, TAB,
    SPACE, BACKSPACE, DEL, ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13
  };

  //  Mapps events to another
class KeyboardState : public GenericSignalMapper<int, int>
{
	public:

    /// a std::shared_ptr of an KeyboardState instance
    typedef std::shared_ptr<KeyboardState > shared_ptr;
    typedef std::shared_ptr<KeyboardState > const_shared_ptr;

    // class factoy
    static shared_ptr create();

    // class destructor
	  virtual ~KeyboardState();

	protected:

    // class constructor
    KeyboardState();

	private:

   // ...
};

} // namespace human_input
} // namespace gloost


#endif // H_GLOOST_HUMANINPUT_KeyboardState

