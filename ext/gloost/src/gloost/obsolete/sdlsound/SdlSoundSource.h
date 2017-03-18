
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



#ifndef GLOOST_SDLSOUNDSOURCE_H
#define GLOOST_SDLSOUNDSOURCE_H



/// gloost system includes
#include <gloost/Point3.h>

/// cpp includes
#include <string>
#include <map>



namespace gloost
{



  class SdlSoundManager;
  class SdlSoundJob;



  ///  Living in the SdlSoundManager this class represents a sounding entity

class SdlSoundSource
{
	public:

    /// class constructor
    SdlSoundSource();

    /// class destructor
	  ~SdlSoundSource();


	  /// this function is called every frame from the SdlSoundManager
	  void frameStep();



    /// check if a Sound job with soundJobId exists within the SdlSoundSource
    bool soundJobExists(unsigned int soundJobId);


    /// get a running SdlSoundJob (be shure the job exists with soundJobExists(unsigned int soundJobId) )
    SdlSoundJob* getSoundJob(unsigned int soundJobId);



    /// starts playing a sound and returns a soundJobId for this play
    unsigned int play(unsigned int soundId,         float volume = 1.0, unsigned int loops = 0);
    unsigned int play(const std::string& soundName, float volume = 1.0, unsigned int loops = 0);


    /// get postion of the sound source in the world
    const Point3& getPosition() const;
    /// set postion of the sound source in the world
    void          setPosition(const Point3& position);


    /// ...
	  const float& getVolume() const;
    /// ...
	  void         setVolume(const float& volume = 1);

    /// -1 = left, 0 = center, 1 = right
	  const float& getPanning() const;
    /// -1 = left, 0 = center, 1 = right
	  void         setPanning(const float& panning = 0);


	  /// returns true if sound job with soundJobId is still playing
	  bool isPlaying(unsigned int soundJobId) const;


	protected:


    /// position of the source
    Point3 _sourcePosition;

    /// over all volume of the source
    float  _sourceVolume;

    /// panning of the source
    float  _sourcePanning;

    /// counts
    unsigned int _frameCounter;


    ///



    /// ...
	  SdlSoundManager* _soundManager;

    /// ...
	  unsigned int _soundJobIdCounter;

	  /// running soundjobs: when the Source is playing a sound it generates a soundjob which exists as long the sound plays
	  std::map<unsigned int, SdlSoundJob*> _runningSoundJobs;

};


} // namespace gloost


#endif // GLOOST_SDLSOUNDSOURCE_H



