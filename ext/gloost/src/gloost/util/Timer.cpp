
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

            gloost is being created by Felix Weißig and Stephan Beck

     Felix Weißig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/

// gloost system includes
#include <gloost/util/Timer.h>

// cpp includes
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace gloost
{
namespace util
{

/**
  \class   Timer

  \brief   Messures the duration of program parts in microseconds

  \author  Felix Weiszig
  \date    May 2013
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////

  /*static*/ Timer::timePointType Timer::_static_now = std::chrono::high_resolution_clock::now();

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

Timer::Timer():
  _timerState(TimerState::STOPPED),
  _duration(0),
  _lastStart(std::chrono::high_resolution_clock::now())
{
//	reset();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

Timer::~Timer()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the messured duration in milliseconds
  \param   ...
  \remarks ...
*/

double
Timer::getDurationInMicro() const
{
  if (_timerState == TimerState::STOPPED)
  {
    return durationTypeMicro(_duration).count();
  }
  return (durationTypeMicro(_duration) + durationTypeMicro(std::chrono::high_resolution_clock::now() - _lastStart)).count();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the messured duration in microseconds
  \param   ...
  \remarks ...
*/

double
Timer::getDurationInMilli() const
{
  if (_timerState == TimerState::STOPPED)
  {
    return durationTypeMilli(_duration).count();
  }
  return (durationTypeMilli(_duration) + durationTypeMilli(std::chrono::high_resolution_clock::now() - _lastStart)).count();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   // returns the seconds of the duration
  \param   ...
  \remarks ...
*/

double
Timer::getDuration() const
{
  if (_timerState == TimerState::STOPPED)
  {
    return durationType(_duration).count();
  }

  return (durationType(_duration) + durationType(std::chrono::high_resolution_clock::now() - _lastStart)).count();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   start time messurement
  \param   ...
  \remarks ...
*/

void
Timer::start()
{
  if (_timerState == TimerState::STOPPED)
  {
    _lastStart  = std::chrono::high_resolution_clock::now();
    _timerState = TimerState::RUNNING;
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   stop time messurement
  \param   ...
  \remarks ...
*/

void
Timer::stop()
{
  if (_timerState == TimerState::RUNNING)
  {
    _duration += std::chrono::high_resolution_clock::now() - _lastStart;
    _timerState = TimerState::STOPPED;
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   resets the timer
  \param   ...
  \remarks ...
*/

void
Timer::reset()
{
  _duration = durationTypeMicro(0);
  _timerState = TimerState::STOPPED;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the current time in seconds
  \param   ...
  \remarks ...
*/

/*static*/
double
Timer::getNowInMilli()
{
  return durationTypeMilli(std::chrono::high_resolution_clock::now() - _static_now).count();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the current time in seconds
  \param   ...
  \remarks ...
*/

/*static*/
double
Timer::getNow()
{
  return durationType(std::chrono::high_resolution_clock::now() - _static_now).count();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the current time in seconds
  \param   ...
  \remarks ...
*/

/*static*/
std::string
Timer::getCurrentTimeString()
{
  #ifdef COMPILER_SUPPORTS_THAT_SHIT
    std::ostringstream stream;
    stream << std::put_time(std::localtime(std::chrono::system_clock::now()), "%F %T");
    return stream.str();
  #endif // COMPILER_SUPPORTS_THAT_SHIT

  std::ostringstream stream;
  std::time_t t = std::time(nullptr);   // get time now
  struct tm* now = localtime(&t);
  stream << (now->tm_year + 1900) << '_'
         << (now->tm_mon + 1) << '_'
         << (now->tm_mday) << '_'
         << (now->tm_hour) << '_'
         << (now->tm_min) << '_'
         << (now->tm_sec);
  return stream.str();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   let the calling thread wait num_milliseconds
  \param   ...
  \remarks ...
*/

/*static*/
void
Timer::wait(size_t num_milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(num_milliseconds));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   puts time like "1.2345 ms" in the outstream
  \param   ...
  \remarks ...
*/

/* extern */
std::ostream&
operator<< (std::ostream& os, const Timer& timer)
{
  os << std::fixed << timer.getDurationInMilli() << " ms";
  return os;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////


} // namespace util
} // namespace gloostTest
