
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



#ifndef GLOOST_NODEDCS_H
#define GLOOST_NODEDCS_H



// gloost includes
#include <gloost/graph/Node.h>
#include <gloost/graph/MatrixCore.h>

#include <gloost/Dcs.h>


// cpp includes


namespace gloost
{
namespace graph
{



/// A Dcs with a Node and a MatrixCore

class NodeDcs: public Dcs
{
	public:
		// class constructor
		NodeDcs();
		/* virtual */ ~NodeDcs();


    /// Add a child node
    void addChild(Node *node);
    void addChild(NodeDcs *nodeDcs);
    /// remove a child node
    void removeChild(Node *node);


    /// set a parent node
    void  setParent  (Node* node);
    void  setParent  (NodeDcs* nodeDcs);
    /// get a parent node
    Node* getParent  () const;


    Node*       getNode() const;
    MatrixCore* getMatrixCore() const;

    void applyToCore();

  private:

   MatrixCore* _core;
   Node*       _node;



};


} // namespace graph
} // namespace gloost


#endif // GLOOST_NODEDCS_H
