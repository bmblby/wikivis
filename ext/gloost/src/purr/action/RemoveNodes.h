
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


#ifndef H_PURR_ACTION_REMOVENODES
#define H_PURR_ACTION_REMOVENODES



// purr includes
#include <purr/action/ActionBase.h>

// gloost includes
#include <gloost/Matrix.h>
#include <gloost/BoundingBox.h>
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>
#include <memory>


namespace purr
{

  namespace model { class Node; }

namespace action
{



  // Provides methodes to remove or/and clean up nodes or subtrees

class RemoveNodes : public ActionBase
{
	public:

    // begins the traversal on a specific node
    static void removeSubtree(const std::shared_ptr<model::Node>& rootOfSubtree);

	protected:

	  static void gatherNodesPostorder(const std::shared_ptr<model::Node>& node,
                                     std::vector< std::shared_ptr<model::Node> >& collectedNodes);

    // class constructor
    RemoveNodes();

    // class destructor
    virtual ~RemoveNodes() = default;

};


} // namespace action
} // namespace purr


#endif // H_PURR_ACTION_REMOVENODES


