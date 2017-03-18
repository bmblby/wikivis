
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_BEHAVIOR_STRINGEVENTDATATOLABEL
#define H_PURR_BEHAVIOR_STRINGEVENTDATATOLABEL



// purr includes
#include <purr/behavior/BehaviorBase.h>
#include <purr/event/common.h>
#include <purr/event/GenericEventData.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace gloost { class MouseState; class vec4; }

namespace purr
{

namespace behavior
{

  // listens to user defined event ids and sets received GenericEventData<std::string> as node label

class StringEventDataToLabel : public BehaviorBase
{
	public:

    const static std::string BehaviorName;

    /// a std::shared_ptr of an StringEventDataToLabel instance
    typedef std::shared_ptr<StringEventDataToLabel>       shared_ptr;
    typedef std::shared_ptr<const StringEventDataToLabel> const_shared_ptr;

    typedef std::unordered_set<purr::event::EventId> EventIdSet;



    // class constructor
    static shared_ptr create(const EventIdSet& eventIdsToListenTo);

    // class destructor
    virtual ~StringEventDataToLabel();



    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);



    // CALLBACKS: called once

    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();

    // ...
    void setLabel(std::shared_ptr<purr::event::EventData> data);



	protected:

    // class constructor
    StringEventDataToLabel(const EventIdSet& eventIdsToListenTo);


	private:

    EventIdSet _eventIdsToListenTo;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_STRINGEVENTDATATOLABEL


