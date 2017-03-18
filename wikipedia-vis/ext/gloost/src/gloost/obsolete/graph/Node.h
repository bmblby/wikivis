
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



#ifndef GLOOST_NODE_H
#define GLOOST_NODE_H



// gloost includes
#include<gloost/Matrix.h>
#include<gloost/BitMask.h>
#include<gloost/graph/Core.h>

// cpp includes
#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include <iostream>


namespace gloost
{
namespace graph
{


  class CoreManager;



 /// A Node of a scenegraph. Has a pointer to a Core

class Node
{
  public:

    typedef std::list<Node*>::iterator       childListIterator;
    typedef std::list<Node*>::const_iterator childListConstIterator;

    Node ();
    Node (Core *core);
    Node (const std::string& nameWithinCoreManager);
    Node (unsigned int coreManagerId);

    ~Node();


    /// Add a child
    void addChild(Node *myNode);

    /// removes a child and returns the iterator to the next child in the list
    void removeChild(Node *myNode);


    /// begin iterator begin
    childListIterator getChildListBegin();
    /// begin iterator end
    childListIterator getChildListEnd();


    /// returns the childlist of the Node
    const std::list<Node*>& getChildList() const;
    std::list<Node*>& getChildList();


    /// returns the number of children of this node
    unsigned int getNumChildren();


    /// set a parent node
    void  setParent  (Node* myNode);
    /// get a parent node
    Node* getParent  () const;


    /// set core
    void setCore (Core* core);
    void setCore (const std::string& nameWithinCoreManager);
    void setCore (unsigned int coreManagerId);

    /// get core
    Core* getCore() const;


    /// swaps the cores of this Node and another
    void swapCores(Node* node);


    /// return matrix with nodes transformations in world coordinates
    const Matrix toWorld() const;


    /// get travMask
    BitMask& getTravMask();

    /// get travMask
    const BitMask& getTravMask() const;

    /// set travMask
    void setTravMask(const BitMask& travMask);


  private:

    /// list of childs
    std::list<Node*> _childList;

    /// parent node
    Node* _parent;

    /// core pointer
    Core *_core;

    /// traversal mask to control the path of the Actions
    BitMask _travMask;

};


} // namespace graph
} // namespace gloost

#endif // GLOOST_NODE_H
