
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

#ifndef H_GLOOST_HUMANINPUT_GENERICSIGNALMAPPER
#define H_GLOOST_HUMANINPUT_GENERICSIGNALMAPPER

// gloost system includes
#include <gloost/gloostConfig.h>

// cpp includes
#include <string>
#include <memory>
#include <unordered_map>
#include <set>

namespace gloost
{
namespace human_input
{


//  Mapps events to another
template <class SourceType, class DestType>
class GenericSignalMapper
{
public:

  /// a std::shared_ptr of an GenericSignalMapper instance
  typedef std::shared_ptr<GenericSignalMapper<SourceType, DestType> >       shared_ptr;
  typedef std::shared_ptr<const GenericSignalMapper<SourceType, DestType> > const_shared_ptr;

  typedef std::unordered_map<SourceType, DestType> MappingsContainer;
  typedef std::set<DestType>                       OccurentsContainer;

  ////////////////////////////////////////////////////////////////////////////////

  // class factoy
  static shared_ptr create()
  {
    auto instance = new GenericSignalMapper<SourceType, DestType>();
    return std::shared_ptr<GenericSignalMapper<SourceType, DestType> >(instance);
  }

  ////////////////////////////////////////////////////////////////////////////////

  // class destructor
  virtual ~GenericSignalMapper()
  {

  }

  ////////////////////////////////////////////////////////////////////////////////

  // inits the GenericSignalMapper
  void
  addMapping(SourceType source, DestType destination)
  {
    _mappings[source] = destination;
  }

  ////////////////////////////////////////////////////////////////////////////////

  // inits the GenericSignalMapper
  void
  removeMapping(SourceType source, DestType destination)
  {
    _mappings.erase[source];
  }

  ////////////////////////////////////////////////////////////////////////////////

  // inits the GenericSignalMapper
  void
  mappingExists(SourceType source)
  {
    return ( _mappings.find(source) != _mappings.end() );
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called once a "frame" to reset events
    \param   ...
    \remarks ...
  */

  const MappingsContainer&
  getMappings() const
  {
    return _mappings;
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called to advance to the next frame
    \param   ...
    \remarks ...
  */

  virtual
  void
  frameStep()
  {
    _thisFramesNewSignals.clear();
    _thisFramesExpiredSignals.clear();
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called if an Event begin
    \param   ...
    \remarks ...
  */

  bool
  setSignal(SourceType sourceSignal)
  {
    auto it = _mappings.find(sourceSignal);
    if (it != _mappings.end())
    {
      const auto& destSignal = it->second;
      // is it a new signal?
      {
//        std::cerr << std::endl << this << ": setSignal:   " << sourceSignal;
        _thisFramesNewSignals.insert(destSignal);
      }
      _currentEvents.insert(destSignal);
      return true;
    }
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called if an Event stopped
    \param   ...
    \remarks ...
  */

  bool
  unsetSignal(SourceType sourceSignal)
  {
    auto sourceSignalIt = _mappings.find(sourceSignal);
    if (sourceSignalIt != _mappings.end())
    {
      const auto& destSignal = sourceSignalIt->second;
      const auto destSignalIt = _currentEvents.find(destSignal);
      if ( destSignalIt != _currentEvents.end() )
      {
//        std::cerr << std::endl << this << ": unsetSignal: " << sourceSignal;

        _thisFramesExpiredSignals.insert(destSignal);
        _currentEvents.erase(destSignalIt);
      }
      return true;
    }
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   returns true if the signal destTypeEntity had started in this frame
    \param   ...
    \remarks ...
  */

  bool
  signalStartedThisFrame(DestType destTypeEntity)
  {
    return _thisFramesNewSignals.find(destTypeEntity) != _thisFramesNewSignals.end();
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   returns true if the signal destTypeEntity was notified in this frame
    \param   ...
    \remarks ...
  */

  bool
  signalExists(DestType destTypeEntity) const
  {
    return _currentEvents.find(destTypeEntity) != _currentEvents.end();
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   returns true if the signal destTypeEntity had started in this frame
    \param   ...
    \remarks ...
  */

  bool
  signalEndedThisFrame(DestType destTypeEntity) const
  {
    return (_thisFramesExpiredSignals.find(destTypeEntity) != _thisFramesExpiredSignals.end());
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called once a "frame" to reset events
    \param   ...
    \remarks ...
  */

  const OccurentsContainer&
  getCurrentEvents() const
  {
    return _currentEvents;
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called once a "frame" to reset events
    \param   ...
    \remarks ...
  */

  const OccurentsContainer&
  getThisFramesNewSignals() const
  {
    return _thisFramesNewSignals;
  }

  ////////////////////////////////////////////////////////////////////////////////

  /**
    \brief   must be called once a "frame" to reset events
    \param   ...
    \remarks ...
  */

  const OccurentsContainer&
  getThisFramesExpiredSignals() const
  {
    return _thisFramesExpiredSignals;
  }

protected:

  // class constructor
  GenericSignalMapper():
    _mappings(),
    _currentEvents()
  {

  }

  ////////////////////////////////////////////////////////////////////////////////

  MappingsContainer   _mappings;
  OccurentsContainer  _currentEvents;
  OccurentsContainer  _thisFramesNewSignals;
  OccurentsContainer  _thisFramesExpiredSignals;

private:

  // ...

};

} // namespace human_input
} // namespace gloost

#endif // H_GLOOST_HUMANINPUT_GENERICSIGNALMAPPER

