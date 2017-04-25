
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


#ifndef H_PURR_BEHAVIOR_PRINTRECEIVEDEVENTS
#define H_PURR_BEHAVIOR_PRINTRECEIVEDEVENTS



// purr includes
#include <purr/behavior/BehaviorBase.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/human_input/MouseState.h>


// cpp includes
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <mutex>

namespace gloost { class MouseState; }

namespace purr
{

namespace behavior
{

  // base class of all behaviors

class PrintReceivedEvents : public BehaviorBase
{
	public:

    const static std::string BehaviorName;

    /// a std::shared_ptr of an PrintReceivedEvents instance
    typedef std::shared_ptr<PrintReceivedEvents>       shared_ptr;
    typedef std::shared_ptr<const PrintReceivedEvents> const_shared_ptr;

    // class constructor
    static shared_ptr create();

    // class destructor
    virtual ~PrintReceivedEvents();

    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);



    // CALLBACKS: called once

    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();


	protected:

    // class constructor
    PrintReceivedEvents();


    void printReceivedEvent(const std::string& eventName);


	private:

	  size_t     _eventCounter;
	  std::mutex _printEventMutex;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_PRINTRECEIVEDEVENTS


