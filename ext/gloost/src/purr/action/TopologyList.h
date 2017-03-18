
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


#ifndef H_PURR_ACTION_TOPOLOGYLIST
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



  // Generates a serialized List of the graph structure

class TopologyList : public ActionBase
{
	public:

    /// an entry within a serialized topology
	  struct ListEntry
	  {
	    ListEntry():_node(nullptr), _depth(0) { }
	    ListEntry(const std::shared_ptr<model::Node>& _node, unsigned depth): _node(_node), _depth(depth) { }
	    std::shared_ptr<model::Node> _node;
	    unsigned _depth;
	  };


    // Generates the List
    static std::vector<ListEntry> generateList(const std::shared_ptr<model::Node>& startNode);

    // prints the list with additional indention for each depth level
    static void printList(std::vector<ListEntry>& list, int indentionWidth = 4);

	protected:


	  static void gatherNodesPreorder(const std::shared_ptr<model::Node>& node,
                                    std::vector<ListEntry>& collectedNodes,
                                    unsigned depth);


    // class constructor
    TopologyList() = delete;

    // class destructor
    ~TopologyList();

};


} // namespace action
} // namespace purr


#endif // H_PURR_ACTION_REMOVENODES


