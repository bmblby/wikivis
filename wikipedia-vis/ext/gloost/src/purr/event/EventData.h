
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


#ifndef H_PURR_EVENT_EVENTBASE
#define H_PURR_EVENT_EVENTBASE



// pur includes
//#include <purr/event/common.h>

// gloost system includes
#include <gloost/gloostConfig.h>


// cpp includes
#include <string>
#include <memory>



namespace purr
{
namespace event
{



  //  Base of all EventsData

class EventData
{
	public:

    /// a std::shared_ptr of an EventData instance
    typedef std::shared_ptr<EventData>       shared_ptr;
    typedef std::shared_ptr<const EventData> const_shared_ptr;

    // class destructor
	  virtual ~EventData();


	protected:

    // class constructor
    EventData();


	private:

};

} // namespace event
} // namespace purr


#endif // H_PURR_EVENT_EVENTBASE


