
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

             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_EVENT_HUMANINPUT
#define H_PURR_EVENT_HUMANINPUT


// cpp includes
#include <string>
#include <memory>
#include <mutex>
#include <queue>



namespace gloost
{
  class Matrix;
  namespace human_input { class MouseState; }
};

namespace purr
{
  namespace model
  {
    class PurrModel;
    class Node;
  }

namespace event
{

  //  uses mouse events to interact with the PurrModel

class HumanInput : public std::enable_shared_from_this<HumanInput>
{
	public:

	  struct KeyStateChange
	  {
	    KeyStateChange(int keyId, int state):
        _keyId(keyId),
        _state(state)
	    {}

      int _keyId;
      int _state;
	  };

    /// a std::shared_ptr of a HumanInput instance
	  typedef std::shared_ptr<HumanInput> shared_ptr;
	  typedef std::shared_ptr<const HumanInput> const_shared_ptr;

	  static const unsigned pickingFlagPosition;

	  // factory
	  static shared_ptr create(std::shared_ptr<model::PurrModel> model);

    // class destructor
    virtual ~HumanInput();

    // performs an interaction step with the gui depending on the mousestate
    void frameStep(const std::shared_ptr<const gloost::human_input::MouseState>& mouseState,
                   const gloost::Matrix& viewMatrix);

    // glfw version of the key up/down event handler
    void keyEventGlfw(int key, int state);

    // returns the Node which received the last mouseDown event
    std::shared_ptr<model::Node> getLastMouseDownNode();

    static void setPickingFlag(const std::shared_ptr<purr::model::Node>& node, bool flag);

	protected:

    // class constructor
    HumanInput(std::shared_ptr<model::PurrModel> model);

    /// the node a mouse down was called in the last iteration (or a nullptr)
    std::shared_ptr<model::Node> _lastDownNode   = nullptr;

    /// the node the mouse was over in the last iteration (or a nullptr)
    std::shared_ptr<model::Node> _lastPickedNode = nullptr;

    /// the node that was dragged in the last iteration (or a nullptr)
    std::shared_ptr<model::Node> _lastDragNode   = nullptr;
    double _timeSinceLastNodeChange              = 0.0;
    std::shared_ptr<model::Node> _hoverNode      = nullptr;

	private:

	  std::shared_ptr<model::PurrModel> _model;

    std::mutex                 _keyChangeQueueMutex;
    std::queue<KeyStateChange> _keyChangeQueue;
};


} // namespace event
} // namespace purr


#endif // H_PURR_EVENT_HUMANINPUT





