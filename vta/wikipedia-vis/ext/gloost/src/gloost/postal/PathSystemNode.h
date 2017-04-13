
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



#ifndef H_GLOOST_PATHSYSTEMNODE
#define H_GLOOST_PATHSYSTEMNODE



// gloost includes
#include <gloost/gloostConfig.h>

// QT includes
#include <string>


// cpp includes
#include <vector>
#include <string>
#include <memory>
//#include <boost/unordered_map.hpp>
#include <unordered_map>
#include <unordered_set>



namespace gloost
{
namespace postal
{

  class Subscriber;



  // Bidirectional node within the path system

class PathSystemNode : public std::enable_shared_from_this<PathSystemNode>
{

public:


  /// a std::shared_ptr of a BinaryBundle instance
  typedef std::shared_ptr<PathSystemNode>       shared_ptr;
  typedef std::shared_ptr<const PathSystemNode> const_shared_ptr;


  typedef std::unordered_map<std::string, std::shared_ptr<PathSystemNode> > ChildNodeContainer;
  typedef std::unordered_set<std::shared_ptr<Subscriber> >                  SubscriberContainer;


  // creates a PathSystemNode and returns a std::shared_ptr to it
  static std::shared_ptr<PathSystemNode> create(const std::string& label,
                                                const std::string& typeString = "type::default",
                                                gloost::gloostId itemGid      = 0);

  // class destructor
  ~PathSystemNode();



  // adds a child note
  bool addChild(std::shared_ptr<PathSystemNode> node);

  // removes a child note
  bool removeChild(std::shared_ptr<PathSystemNode> node);

  // returns nodes parent or a nullptr
  std::shared_ptr<PathSystemNode> getParent() const;

  // returns a child note with a particular label or a null_ptr if the child node was not found
  std::shared_ptr<PathSystemNode> getChild(const std::string& childsLabel) const;

  // returns a std::vector of child nodes
  ChildNodeContainer&       getChildren();
  const ChildNodeContainer& getChildren() const;



  // add a subscriber to this PathSystemNode
  virtual bool addSubscriber(std::shared_ptr<Subscriber> subscriber);

  // remove a subscriber from this PathSystemNode
  virtual bool removeSubscriber(std::shared_ptr<Subscriber> subscriber);

  // returns true
  virtual bool hasSubscriber(std::shared_ptr<Subscriber> subscriber) const;

  // returns a container with the Subscribers of this node

  virtual const SubscriberContainer& getSubscribers() const;


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



  // returns the depth of this node within a tree
  unsigned getDepth() const;



protected:

  // class contructor
  PathSystemNode(const std::string& label,
                 const std::string& typeString,
                 gloost::gloostId itemGid);



  private:

    // sets the parent of this node
    void setParent(std::shared_ptr<PathSystemNode> node);


    std::weak_ptr<PathSystemNode> _parent;
    ChildNodeContainer            _children;
    std::string                   _label;
    std::string                   _typeString;
    gloost::gloostId              _itemGid;


    // set of subscribers for this path
    SubscriberContainer _subscribers;


};


} // namespace postal
} // namespace gloost


#endif // H_GLOOST_PATHSYSTEMNODE
