
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



#ifndef GLOOST_SDLSOUNDJOB_H
#define GLOOST_SDLSOUNDJOB_H



/// gloost system includes


/// cpp includes
#include <string>



namespace gloost
{


  ///  represents a sound while it is playing

class SdlSoundJob
{
	public:

    /// class constructor
    SdlSoundJob( unsigned int mySoundJobId,
                 unsigned int soundId,
                 float        volume,
                 unsigned int loops,
                 unsigned int channel);

    /// class constructor
    SdlSoundJob(const SdlSoundJob& job);




    /// class destructor
	  ~SdlSoundJob();


    unsigned int _soundJobId;
    unsigned int _soundId;
    float        _volume;
    float        _loops;
    unsigned int _channel;
    bool         _halted;
    unsigned int _expireInMs;
    bool         _paused;

	private:


};


} // namespace gloost


#endif // GLOOST_SDLSOUNDJOB_H


