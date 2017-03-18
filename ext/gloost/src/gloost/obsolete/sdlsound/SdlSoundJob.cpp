
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



/// gloost system includes
#include <gloost/sdlsound/SdlSoundJob.h>



/// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

////////////////////////////////////////////////////////////////////////////////


  /// class constructor

SdlSoundJob::SdlSoundJob(unsigned int mySoundJobId,
                         unsigned int soundId,
                         float        volume,
                         unsigned int loops,
                         unsigned int channel):
    _soundJobId(mySoundJobId),
    _soundId(soundId),
    _volume(volume),
    _loops(loops),
    _channel(channel),
    _halted(false),
    _paused(false)
{

}

////////////////////////////////////////////////////////////////////////////////


  /// class constructor

SdlSoundJob::SdlSoundJob(const SdlSoundJob& job):
    _soundJobId(job._soundJobId),
    _soundId(job._soundId),
    _volume(job._volume),
    _loops(job._loops),
    _channel(job._channel),
    _halted(false),
    _paused(false)
{

}


////////////////////////////////////////////////////////////////////////////////


  /// class destructor

SdlSoundJob::~SdlSoundJob()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


  /// ...







////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


