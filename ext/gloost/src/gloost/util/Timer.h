
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

#ifndef H_GLOOST_TIMER
#define H_GLOOST_TIMER

// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <chrono>

namespace gloost
{
namespace util
{
  //  Messures the duration of program parts in microseconds

class Timer
{
	public:

	  typedef std::chrono::duration<double, std::micro>  durationTypeMicro;
	  typedef std::chrono::duration<double, std::milli>  durationTypeMilli;
	  typedef std::chrono::duration<double>              durationType;
	  typedef std::chrono::time_point<std::chrono::high_resolution_clock> timePointType;

    // class constructor
    Timer();

    // class destructor
	  virtual ~Timer();


    // returns the messured duration in microseconds
    double getDurationInMicro() const;

    // returns the messured duration in milliseconds
    double getDurationInMilli() const;

    // returns the messured duration in seconds
    double getDuration() const;


    // start time messurement
	  void start();

    // start time messurement
	  void stop();

    // resets the timer
    void reset();


    // returns the time from program start until now in milliseconds
    static double getNowInMilli();

    // returns the time from program start until now in seconds
    static double getNow();

    // returns the time from program start until now in seconds
    static std::string getCurrentTimeString();

    // let the calling thread wait num_milliseconds
    static void wait(size_t num_milliseconds);

	private:

    enum class TimerState: unsigned char
    {
      STOPPED,
      RUNNING
    };

    TimerState        _timerState;
    durationTypeMicro _duration;
    timePointType     _lastStart;

    static timePointType _static_now;
};

/// ostream operator
extern std::ostream& operator<< (std::ostream&, const Timer&);

} // namespace util
} // namespace gloost

#endif // H_GLOOST_TIMER
