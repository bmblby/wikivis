
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



#ifndef GLOOST_ACTION_H
#define GLOOST_ACTION_H

#include <gloost/graph/Node.h>
#include <gloost/graph/Collector.h>
#include <gloost/BitMask.h>


namespace gloost
{

namespace graph
{


  ///  Action base class. An Action in gloost is a scenegraph traverser

class Action
{
	public:
    /// class constructor with contextId = 0;
    Action();

    /// class constructor
    Action(int contextId);

    /// class destructor
    virtual ~Action();



    /// Run and return last Node visited
    virtual void start (Node* startNode);



    /// get the travMask
    const BitMask& getTravMask () const;
    BitMask& getTravMask ();

    /// set travMask
    void setTravMask (const BitMask& travMask);



    /// get BitMask with flags the Action can modify
    const BitMask& getWorkMask() const;
    BitMask& getWorkMask();

    /// set BitMask with flags the Action can modify
    void setWorkMask(const BitMask& workMask);



    /// get the collector
    Collector* getCollector();
    /// set a collector
    void       setCollector(Collector *collector);



    /// get contextId
    unsigned int getContextId() const;

    /// set contextId
    void setContextId(unsigned int contextId);



    /// get contextId
    Node* getCurrentNode() const;



	private:


    ///


  protected:

    ///
    BitMask _travMask;

    ///
    BitMask _workMask;

    /// attache a collector to collect statistic data of a traversal pass
    Collector* _collector;

    /// multi gpu - multi context
    unsigned int _contextId;

    /// the node the action is handling in this particular moment
    Node*        _currentNode;



    /// set the node the action is currently working on during traversal
    void setCurrentNode(Node* currentNode);



};


} // namespace graph
} // namespace gloost


#endif // GLOOST_ACTION_H
