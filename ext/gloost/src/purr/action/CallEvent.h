
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_ACTION_CALLEVENT
#define H_PURR_ACTION_CALLEVENT



// purr includes
#include <purr/action/ActionBase.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost { class MouseState; }


namespace purr
{

  namespace event { class EventData; }


namespace action
{



  // Traverses the graph twice for calling enterFrame and exitFrame

class CallEvent : public ActionBase
{
	public:

    // class destructor
    virtual ~CallEvent();


    /* Note: This static methodes should be temporaly. A traverser with a "traversal strategy" and
             a "call strategy" is in order to be flexible enough. This could be called as static
             methode too though.
    */

    // traverses the tree inorder and calls event with <eventName> on all nodes behaviors
    static void callPreorder(const std::shared_ptr<model::Node>& startNode,
                            const std::string& eventName,
                            const std::shared_ptr<event::EventData>& data = nullptr);

    // calles an event with timeInMilleSeconds delay
    static void callPreorderLater(const std::shared_ptr<model::Node>& startNode,
                                  const std::string& eventName,
                                  unsigned milliseconds,
                                  const std::shared_ptr<event::EventData>& data = nullptr);

    // calls an event on all behavior of on node
    static void call(const std::shared_ptr<model::Node>& node,
                     const std::string& eventName,
                     const std::shared_ptr<event::EventData>& data = nullptr);

    // calls an event on all behavior of on node with timeInMilleSeconds delay
    static void callLater( const std::shared_ptr<model::Node>& startNode,
                           const std::string& eventName,
                           unsigned milliseconds,
                           const std::shared_ptr<event::EventData>& data = nullptr);


    // calls an event on all behavior of on node and all its predecessor
    static void callUpwards(const std::shared_ptr<model::Node>& node,
                            const std::string& eventName,
                            const std::shared_ptr<event::EventData>& data = nullptr);


    // calls an event on all behavior of on node and all its predecessor with timeInMilleSeconds delay
    static void callUpwardsLater(const std::shared_ptr<model::Node>& node,
                                 const std::string& eventName,
                                 unsigned milliseconds,
                                 const std::shared_ptr<event::EventData>& data = nullptr);

    // calls an event on all behavior of direct subnodes of node
    static void callSubnodes(const std::shared_ptr<model::Node>& node,
                             const std::string& eventName,
                             const std::shared_ptr<event::EventData>& data = nullptr);

    // calls an event on all behavior of all siblings of node
    static void callSiblings(const std::shared_ptr<model::Node>& node,
                             const std::string& eventName,
                             const std::shared_ptr<event::EventData>& data = nullptr);


	protected:

    CallEvent();



	private:




};


} // namespace action
} // namespace purr


#endif // H_PURR_ACTION_CALLEVENT


