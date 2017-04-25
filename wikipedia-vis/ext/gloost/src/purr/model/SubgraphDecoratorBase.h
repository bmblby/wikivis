
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_MODEL_SUBGRAPHDECORATORBASE
#define H_PURR_MODEL_SUBGRAPHDECORATORBASE



// purr includes
#include <purr/model/Node.h>


// gloost system includes
#include <gloost/gloostConfig.h>


// cpp includes
#include <string>
#include <memory>
#include <unordered_map>

namespace purr
{

namespace model
{

  class PurrModel;

  //  wraps a subgraph and decorates it with additional functionality

class SubgraphDecoratorBase : public std::enable_shared_from_this<SubgraphDecoratorBase>
{
	public:

    /// a std::shared_ptr of an SubgraphDecoratorBase instance
    typedef std::shared_ptr<SubgraphDecoratorBase>       shared_ptr;
    typedef std::shared_ptr<const SubgraphDecoratorBase> const_shared_ptr;

    typedef std::unordered_map<std::string, std::shared_ptr<model::Node> > NodeContainer;


    // class constructor
//    static shared_ptr create();

    // class destructor
    virtual ~SubgraphDecoratorBase();

    // returns the root node
    std::shared_ptr<model::Node>       getSubgraphRootNode();
    std::shared_ptr<const model::Node> getSubgraphRootNode() const;

    // returns a node by its given name within this decorator or a nullptr
    std::shared_ptr<Node> getNode(const std::string& nameWithinDecorator) const;

    // removes the widget from the tree, i.e.: removes the widgetRoot from its parent
    void removeFromTree();


    // adds a node as subnode to the root of this subgraph
    static bool attachSubGraph(std::shared_ptr<Node> parentNode,  shared_ptr subgraph);


	protected:

    // class constructor
    SubgraphDecoratorBase( const std::shared_ptr<model::PurrModel>& purrModel,
                           const std::string& type = "",
                           const std::string& label = "");

    // adds a association of a name with a node, properly in the current SubgraphDecoratorBase instance (or is it?)
    void addNode(const std::string& name, const std::shared_ptr<model::Node>& node);

    // returns the Nodes this widget consists of
    NodeContainer&       getNodes();
    const NodeContainer& getNodes() const;

    // sets the root node
    void setSubgraphRootNode(const std::shared_ptr<model::Node>& node);

	private:

    /// nodes of the subgraph this decorator decorates
	  NodeContainer                _nodes;
    std::shared_ptr<model::Node> _subgraphRootNode;


};

} // namespace model
} // namespace purr


#endif // H_PURR_MODEL_SUBGRAPHDECORATORBASE
