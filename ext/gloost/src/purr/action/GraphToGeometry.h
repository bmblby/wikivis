
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

#ifndef H_PURR_VISUALIZE_GRAPHTOGEOMETRY
#define H_PURR_VISUALIZE_GRAPHTOGEOMETRY

// gloost system includes
#include <gloost/gloostConfig.h>
#include <purr/action/ActionBase.h>
#include <purr/visualize/VisNode.h>

// cpp includes
#include <string>
#include <memory>

namespace purr
{
  namespace model     { class Node; }
  namespace visualize { class VisNode; }

namespace action
{

  //  Build Geometry to visualize a subtree of the purr graph
class GraphToGeometry : public ActionBase
{
	public:

    /// a std::shared_ptr of an GraphToGeometry instance
    typedef std::shared_ptr<GraphToGeometry>       shared_ptr;
    typedef std::shared_ptr<const GraphToGeometry> const_shared_ptr;

    static shared_ptr create();
	  virtual ~GraphToGeometry();

    // rebuilds the graphical representation of the graph
	  void begin( const std::shared_ptr<model::Node>& startNode );

    // arrange the nodes to be nice and tidy
	  void arrangeNodes(  );

	protected:

	  void buildGraph( const std::shared_ptr<visualize::VisNode>& currentNode, unsigned depth );

    // class constructor
    GraphToGeometry();

	private:

    unsigned _graphDepth;
    unsigned _numNodes;
    std::shared_ptr<visualize::VisNode> _rootVisNode;
    std::vector< std::vector< std::shared_ptr<visualize::VisNode> > > _nodesPerLevel;

};

} // namespace action
} // namespace purr

#endif // H_PURR_VISUALIZE_GRAPHTOGEOMETRY

