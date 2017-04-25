
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

#ifndef H_PURR_EVENT_GENERIC_EVENT_DATA
#define H_PURR_EVENT_GENERIC_EVENT_DATA

// pur includes
#include <purr/event/common.h>
#include <purr/event/EventData.h>

// gloost system includes
#include <gloost/gloostConfig.h>

// cpp includes
#include <string>
#include <memory>

namespace purr
{
namespace event
{

  //  GenericEventData template

template <class ContentType = gloost::gloostId>
class GenericEventData : public EventData
{
	public:

    /// a std::shared_ptr of an GenericEventData instance
    typedef std::shared_ptr<GenericEventData<ContentType> >       shared_ptr;
    typedef std::shared_ptr<const GenericEventData<ContentType> > const_shared_ptr;

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   Class factory
      \remarks ...
    */
    /// uses existing shared_ptr
    static shared_ptr create(const std::shared_ptr<ContentType>& content)
    {
      return shared_ptr(new GenericEventData<ContentType>(content));
    }

    /// generates a shared_ptr internaly
    static shared_ptr create(ContentType* content)
    {
      return shared_ptr(new GenericEventData<ContentType>(std::shared_ptr<ContentType>(content)));
    }

    /// coppies the content and stores it internaly using a shared_ptr
    static shared_ptr create(const ContentType& content)
    {
      return shared_ptr(new GenericEventData<ContentType>(std::shared_ptr<ContentType>( new ContentType(content) )));
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   Class destructor
      \remarks ...
    */

	  virtual ~GenericEventData()
    {
      // insert your code here
    }

    ///////////////////////////////////////////////////////////////////////////


    /**
      \brief   returns a shared_ptr to the content
      \remarks ...
    */

    std::shared_ptr<ContentType> getContent()
    {
      return _content;
    }

    /**
      \brief   returns a shared_ptr to the content
      \remarks ...
    */

    std::shared_ptr<const ContentType> getContent() const
    {
      return _content;
    }


    ///////////////////////////////////////////////////////////////////////////


    /**
      \brief   returns an EventData pointer for a GenericEventData<ContentType>::shared_ptr or a nullptr
      \remarks ...
    */

    static shared_ptr castToThisType(const std::shared_ptr<EventData>& baseEvent)
    {
       return std::dynamic_pointer_cast< GenericEventData<ContentType> >(baseEvent);
    }

    static shared_ptr castToThisType(const std::shared_ptr<const EventData>& baseEvent)
    {
       return std::dynamic_pointer_cast< GenericEventData<ContentType> >(baseEvent);
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns an EventData pointer for a GenericEventData<ContentType>::shared_ptr or a nullptr
      \remarks ...
    */

    static shared_ptr static_castToThisType(const std::shared_ptr<EventData>& baseEvent)
    {
       return std::static_pointer_cast< GenericEventData<ContentType> >(baseEvent);
    }

    static shared_ptr static_castToThisType(const std::shared_ptr<const EventData>& baseEvent)
    {
       return std::static_pointer_cast< GenericEventData<ContentType> >(baseEvent);
    }

	protected:

    /**
      \brief   Class constructor
      \remarks ...
    */

    GenericEventData(const std::shared_ptr<ContentType>& content):
      EventData(),
      _content(content)
    {
      // insert your code here
    }

    ///////////////////////////////////////////////////////////////////////////

	private:

    std::shared_ptr<ContentType> _content;

};


/// ostream operator
template <class ContentType>
extern std::ostream& operator<< (std::ostream& os, const std::shared_ptr<const GenericEventData<ContentType> >& ge)
{
  os << (purr::event::EventData::const_shared_ptr) ge;
  return os;
}

/// ostream operator
template <class ContentType>
extern std::ostream& operator<< (std::ostream& os, const std::shared_ptr<GenericEventData<ContentType> >& ge)
{
  os << (purr::event::EventData::const_shared_ptr) ge;
  return os;
}

} // namespace event
} // namespace purr


#endif // H_PURR_EVENT_GENERIC_EVENT_DATA


