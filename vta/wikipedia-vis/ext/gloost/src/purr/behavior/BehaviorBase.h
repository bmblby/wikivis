
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


#ifndef H_PURR_BEHAVIOR_BEHAVIORBASE
#define H_PURR_BEHAVIOR_BEHAVIORBASE


// purr includes
#include <purr/event/common.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/human_input/MouseState.h>


// cpp includes
#include <string>
#include <memory>
#include <functional>
#include <mutex>
#include <unordered_map>


namespace gloost { class MouseState; }

namespace purr
{

  namespace model { class PurrModel; class Node; }
  namespace event { class EventData; }


namespace behavior
{

  // type of a function that receives data
  typedef std::function<void(std::shared_ptr<event::EventData>)> EventFunctionType;

  // base class of all behaviors

class BehaviorBase
{
	public:


//    /// a std::shared_ptr of an BehaviorBase instance
    typedef std::shared_ptr<BehaviorBase>                       shared_ptr;
    typedef std::shared_ptr<const BehaviorBase>                 const_shared_ptr;
    typedef std::unordered_map<std::string,  EventFunctionType> EventMap;

    // class constructor
//    create(const std::string& name, const std::shared_ptr<model::Node>& ownerNode);

    // class destructor
    virtual ~BehaviorBase();

    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);


    // returns the owner of the node
    std::shared_ptr<model::Node>       getNode();
    std::shared_ptr<const model::Node> getNode() const;

    // returns the name of the behavior
    const std::string& getName() const;


    // is called when the behavior is assigned to a Node
    virtual void onAssignToNode(const std::shared_ptr<model::Node>& node);

    // is called when the behavior is added to a model::Node
    virtual void onBeginBehavior();

    // is called when the behavior is removed from model::Node
    virtual void onEndBehavior();



    // maps an event::EventId to a std::function
    void mapEvent(const event::EventId& eventId, std::function<void(std::shared_ptr<event::EventData>)>);

    // unmaps an event::EventId
    void unmapEvent(const event::EventId& eventId);

    // unmaps all events
    void unmapAllEvents();


    // calls a mouse event
    void callEvent(const event::EventId& eventId, const std::shared_ptr<event::EventData>& data = nullptr);


	protected:
    // class constructor
    BehaviorBase(const std::string& name);

    // returns the base mutex used to lock the behavior
    std::mutex& getBaseMutex();

	private:

	  std::string                _name;
	  std::weak_ptr<model::Node> _ownerNode;

    /// map of event name -> std::function
    EventMap _events;

    // default mutex to lock the whole behavior
    std::mutex _readWriteChangeMutex;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_BEHAVIORBASE


