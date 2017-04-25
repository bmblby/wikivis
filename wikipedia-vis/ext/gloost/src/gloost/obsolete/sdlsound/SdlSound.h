
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



#ifndef GLOOST_SDLSOUND_H
#define GLOOST_SDLSOUND_H



/// gloost system includes
#include <gloost/SharedResource.h>



/// cpp includes
#include <string>

#ifdef GLOOST_GNULINUX
  #include <SDL/SDL.h>
  #include <SDL/SDL_mixer.h>
#else
  #include <SDL.h>
  #include <SDL_mixer.h>
#endif



namespace gloost
{


  class SdlSoundManager;



  ///  Wrapper Mix_Chunk of SDL_MIXER

class SdlSound: public SharedResource
{
	public:
    /// class constructor
    SdlSound(const std::string& filePath);

    /// class destructor
	  ~SdlSound();


	  /// loads the soundfile
	  bool loadFile(const std::string& filePath);

	  /// unloads the soundfile
	  void unloadFile();


	  /// start playing the sample and return the channel
	  int play( float volume  = 1.0,
              int   repeat  = 0,
              int   channel = -1);



    float getGeneralVolume() const;
    void  setGeneralVolume(const float generalVolume = 1.0);


    Mix_Chunk* getChunk();
    const Mix_Chunk* getChunk() const;


	protected:


    /// ...
    SdlSoundManager* _soundManager;


    /// ...
	  std::string _filePath;

	  /// ...
	  float       _generalVolume;

    /// the sdl_mixer sample container
    Mix_Chunk* _soundChunk;


};


} // namespace gloost


#endif // GLOOST_SDLSAMPLE_H


