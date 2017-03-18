
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

#ifndef H_PURR_MODEL_PURRMODEL
#define H_PURR_MODEL_PURRMODEL

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/Point3.h>
#include <gloost/Matrix.h>
#include <gloost/human_input/MouseState.h>

// cpp includes
#include <string>
#include <memory>
#include <map>
#include <unordered_set>

namespace gloost { class Matrix; class Point3; }

namespace purr
{

namespace model
{
  class Node;

  // Gui model

class PurrModel : public std::enable_shared_from_this<PurrModel>
{
	public:

    /// a std::shared_ptr of an PurrModel instance
    typedef std::shared_ptr<PurrModel>       shared_ptr;
    typedef std::shared_ptr<const PurrModel> const_shared_ptr;

    // creator
    static shared_ptr create();

    // class destructor
    virtual ~PurrModel();

    // returns the root of the gui elements
    std::shared_ptr<Node> getRootNode();


    // sets the current mouse state to be accessable by Behaviors
    virtual void setCurrentMouseState(const std::shared_ptr<const gloost::human_input::MouseState>& mouseState);

    // returns the current mouse state
    virtual std::shared_ptr<const gloost::human_input::MouseState> getCurrentMouseState() const;

	protected:

    // class constructor
    PurrModel();

    // set/initializes the root node
    void setRootNode(const std::shared_ptr<Node>& node);


    /// root of the tree of Node instances
    std::shared_ptr<Node> _rootNode;

    /// current mouse state
    std::shared_ptr<const gloost::human_input::MouseState> _mouseState;

	private:

};

} // namespace model
} // namespace purr

#endif // H_PURR_MODEL_PURRMODEL

