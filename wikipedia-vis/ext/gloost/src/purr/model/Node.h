
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


#ifndef H_LIBPURR_MODEL_NODE
#define H_LIBPURR_MODEL_NODE


// gloost includes
#include <gloost/gloostConfig.h>
#include <gloost/BoundingBox.h>
#include <gloost/BitMask.h>

// QT includes
#include <string>


// cpp includes
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <boost/unordered_map.hpp>


namespace gloost { class Point3; class Vector3; }

namespace purr
{

  namespace behavior { class BehaviorBase; }



namespace model
{

  class PurrModel;


  // Bidirectional node with a label and stuff used in models for TreeViews

class Node : public std::enable_shared_from_this<Node>
{

  public:

    /// a std::shared_ptr of an GraphicalModel instance
    typedef std::shared_ptr<Node>       shared_ptr;
    typedef std::shared_ptr<const Node> const_shared_ptr;

    // container type for children
    typedef std::vector<shared_ptr> SubnodeContainer;

    // pointer to parent
    typedef std::weak_ptr<Node>       weak_ptr;
    typedef std::weak_ptr<const Node> const_weak_ptr;

    // containers of behaviors
    typedef std::vector<std::shared_ptr<behavior::BehaviorBase> >                       BehaviorVector;
    typedef boost::unordered_map<std::string, std::shared_ptr<behavior::BehaviorBase> > BehaviorMap;

    // factory
    static shared_ptr create(const std::shared_ptr<PurrModel>& purrModel,
                             const std::string& label          = "",
                             const std::string& typeString     = "Node::default",
                             gloost::gloostId   itemGid        = 0);

    // class destructor
    virtual ~Node();


    // returns the assigned PurrModel instance or a nullptr
    std::shared_ptr<PurrModel>       getPurrModel();
    std::shared_ptr<const PurrModel> getPurrModel() const;


    // ### Tree ###################################

    // adds a node as subnode
    void addSubnode(const shared_ptr& node);

    // creates Node, adds it as subnode and returns a shared ptr to it
    Node::shared_ptr addSubnode();

    // creates Node, adds it as subnode and returns a shared ptr to it
    Node::shared_ptr addSubnode(const std::string& label);

    // removes a child note
    bool removeSubnode(const shared_ptr& node);

    // remove all subnodes
    void removeAllSubnodes();

    // returns nodes parent or a nullptr
    shared_ptr getParentNode() const;

    // returns this nodes position within parents vector of children
    int getIndexWithinParentNode() const;

    // returns a child note with a particular position within this nodes children
    shared_ptr getSubnode(unsigned index);

    shared_ptr getSubnodeByLabel(const std::string& label);

    // returns a std::vector of child nodes
    SubnodeContainer&       getSubnodes();
    const SubnodeContainer& getSubnodes() const;


    // moves a subnode to the last position in the SubnodeContainer of this node
    void moveToSubNodesEnd(const shared_ptr& subnode);


    // returns true if this node is a root
    bool isRoot() const;

    // returns true if this node is a leaf
    bool isLeaf() const;

    // return the root node of the tree this node exists in
    shared_ptr getRootNode();

    // returns the depth of this node within a tree
    unsigned getDepth() const;


    // ### behaviors ########################################

    // adds a behavior to the Node
    void addBehavior(const std::shared_ptr<behavior::BehaviorBase>& behavior);

    // removes a behavior to the Node
    void removeBehavior(const std::shared_ptr<behavior::BehaviorBase>& behavior);
    void removeBehavior(const std::string& behaviorName);

    // removes all behaviors
    void removeBehaviors();

    // returns a vector of behaviors in call order
    BehaviorVector&       getBehaviorsByCallOrder();
    const BehaviorVector& getBehaviorsByCallOrder() const;

    // returns a behavior by name or a nullptr
    std::shared_ptr<behavior::BehaviorBase> getBehavior(const std::string& behaviorName);


    // ### Properties ###############################

    // returns the label of the node
    const std::string& getLabel() const;
    // sets the label of this node
    void setLabel(const std::string& label);


    // returns the type string of this node
    const std::string& getTypeString() const;
    // sets the type string of this node
    void setTypeString(const std::string& typeString);


    // returns the item gid of this node
    gloost::gloostId  getItemGid() const;
    // sets the item gid of this node
    void setItemGid(gloost::gloostId gid);


    // ### Geometry ##################################

    // sets the position (lower left corner)
    void setPosition (const gloost::Point3& position);
    // returns the position (lower left corner)
    gloost::Point3 getPosition() const;

    // moves the node
    void move(const gloost::Vector3& speed);

    // sets the position (lower left corner)
    void setSize(const gloost::Vector3& size);
    // returns the position (lower left corner)
    gloost::Vector3 getSize() const;

    // returns the gloost::BoundingBox which defines the area of this entity
    const gloost::BoundingBox& getAabb() const;


    // returns the to world matrix
    gloost::Matrix getToWorldMatrix() const;

    // transforms a point from world space to Node space
    gloost::Point3 fromWorldToNodeSpace(const gloost::Point3& point) const;

    // transforms a vector from world space to Node space
    gloost::Vector3 fromWorldToNodeSpace(const gloost::Vector3& vector) const;

    // sorts the container of child nodes their label
    void sortChildrenByLabel();

    //
    const gloost::BitMask& getTravMask() const;
    void setTravMask(const gloost::BitMask&);

  protected:

    // class contructor
    Node( const std::shared_ptr<PurrModel>& purrModel,
             const std::string& label,
             const std::string& typeString,
             gloost::gloostId   itemGid);
  private:

    // sets the parent of this node
    void setParentNode(const shared_ptr& node);

    std::weak_ptr<PurrModel> _purrModel;

    std::string      _label;
    std::string      _typeString;
    gloost::gloostId _itemGid;

    //
    BehaviorVector _behaviorsByCallOrder;
    BehaviorMap    _behaviorsByName;

    weak_ptr            _parentNode;
    SubnodeContainer    _subNodes;


    gloost::BoundingBox _aabb;
    gloost::BitMask     _travMask;


};


} // namespace model
} // namespace purr


#endif // H_LIBPURR_MODEL_NODE
