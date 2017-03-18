
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
#include <gloost/sdlsound/SdlSoundManager.h>



/// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

SdlSound::SdlSound(const std::string& filePath):
    SharedResource(),
    _soundManager(SdlSoundManager::getInstance()),
    _filePath(filePath),
    _generalVolume(0.9),
    _soundChunk(0)
{
	loadFile(filePath);
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

SdlSound::~SdlSound()
{
	unloadFile();
}


///////////////////////////////////////////////////////////////////////////////


  /// loads the soundfile

bool
SdlSound::loadFile(const std::string& filePath)
{

  _filePath = filePath;

  if (_soundChunk)
  {
    unloadFile();
  }


  // Load our sound files from disk
  _soundChunk = Mix_LoadWAV(filePath.c_str());

  if (_soundChunk)
  {
    std::cout << std::endl;
    std::cout << std::endl << "Message from SdlSound::loadFile(const std::string& filePath):" ;
    std::cout << std::endl << "             Sound file " << filePath << " successfully loaded.";
    std::cout << std::endl;
    return true;
  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in SdlSound::loadFile(const std::string& filePath):" ;
    std::cout << std::endl << "         Could NOT load sound file " << filePath << " !";
    std::cout << std::endl;
    return false;
  }

}


///////////////////////////////////////////////////////////////////////////////


  /// unloads the soundfile

void
SdlSound::unloadFile()
{
  if (_soundChunk)
  {
    Mix_FreeChunk(_soundChunk);
    _soundChunk = 0;
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// start playing the sample and return the channel

int
SdlSound::play( float volume,
                int   repeat,
                int   channel)

{

  /// play the sound
  int usedChannel = Mix_PlayChannel(channel, _soundChunk, repeat);

  /// check if channel was given
  if (usedChannel == -1)
  {
    std::cout << std::endl;
    std::cout << std::endl << "Warning from SdlSound::play(float volume,int repeat, unsigned int channel):" ;
    std::cout << std::endl << "             Out of channels while trying to play a SdlSound.";
    std::cout << std::endl;
    std::flush(std::cout);
  }
  else
  {

    /// set the volume of the channel
    Mix_Volume(usedChannel, (int)(_generalVolume*volume*128));
  }

  return usedChannel;
}


///////////////////////////////////////////////////////////////////////////////


  /// ...

float
SdlSound::getGeneralVolume() const
{
  return _generalVolume;
}


///////////////////////////////////////////////////////////////////////////////


  /// ...

void
SdlSound::setGeneralVolume(const float generalVolume)
{
  _generalVolume = generalVolume;
}


///////////////////////////////////////////////////////////////////////////////


  /// ...

Mix_Chunk*
SdlSound::getChunk()
{
  return _soundChunk;
}


///////////////////////////////////////////////////////////////////////////////


  /// ...

const Mix_Chunk*
SdlSound::getChunk() const
{
  return _soundChunk;
}


///////////////////////////////////////////////////////////////////////////////





} // namespace gloost


