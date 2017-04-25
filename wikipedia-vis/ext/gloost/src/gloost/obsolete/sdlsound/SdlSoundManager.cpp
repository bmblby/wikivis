
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



/// gloost includes
#include <gloost/sdlsound/SdlSoundManager.h>

/// cpp/std includes
#include <iostream>
#include <vector>


namespace gloost
{


 /*static*/
 SdlSoundManager* SdlSoundManager::_theinstance = 0;

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

SdlSoundManager::SdlSoundManager():
    _soundIdCounter(0),
    _sounds(),
    _assignMap(),
    _unusedChannelsStack(),
    _soundSources(),
    _sourceIdCounter(0),
    _numMixChannels(16),
    _sampleRate(44100),
    _audioFormat(AUDIO_S16SYS),
    _numChannels(2),
    _sampleBuffer(2048)
{


}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

SdlSoundManager::~SdlSoundManager()
{
//  getCoreWithoutRefcount(0)->dropReference();
  cleanUp();
}


///////////////////////////////////////////////////////////////////////////////


  /// get an instance to the SdlSoundManager

SdlSoundManager* const
SdlSoundManager::getInstance()
{
  if(!_theinstance)
  {
    _theinstance = new SdlSoundManager();
  }

  return _theinstance;
}


////////////////////////////////////////////////////////////////////////////////


  /// configures and starts sdl_mixer (call ths after SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) | ...)

bool
SdlSoundManager::start( unsigned int numMixChannels,
                        unsigned int sampleRate,
                        Uint16       audioFormat,
                        unsigned int numChannels,
                        unsigned int sampleBuffer)

{
  _numMixChannels = numMixChannels;
  _sampleRate    = sampleRate;
  _audioFormat   = audioFormat;
  _numChannels   = numChannels;
  _sampleBuffer  = sampleBuffer;


  // Initialize SDL_mixer with our chosen audio settings
  if (Mix_OpenAudio(sampleRate, audioFormat, numChannels, sampleBuffer) != 0)
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in SdlSoundManager::start( unsigned int numMixChanels, unsigned int sampleRate, ... :" ;
    std::cout << std::endl << "         Mix_openAudio of SDL_mixer returns with an error:";
    std::cout << std::endl <<  SDL_GetError();
    std::cout << std::endl;
    return false;
  }



  std::cout << std::endl;
  std::cout << std::endl << "Message from SdlSoundManager::start( unsigned int numMixChanels, unsigned int sampleRate, ... :" ;
  std::cout << std::endl;
  std::cout << std::endl << "   Sdl_mixer is up and running, using " << _numMixChannels << " channels.";
  std::cout << std::endl;


  Mix_AllocateChannels(_numMixChannels);


  /// reset all containers within the SdlSoundManager
  this->reset();

}


////////////////////////////////////////////////////////////////////////////////


  /// call this function every frame

void
SdlSoundManager::frameStep()
{


  /// call framestep on all sources
  std::map<unsigned int, SdlSoundSource*>::iterator sourceIt    = _soundSources.begin();
  std::map<unsigned int, SdlSoundSource*>::iterator sourceEndIt = _soundSources.end();


  for (; sourceIt != sourceEndIt; ++sourceIt)
  {
    (*sourceIt).second->frameStep();
  }


}


////////////////////////////////////////////////////////////////////////////////


  /// close the soundmanager, delete all sound resources

void
SdlSoundManager::reset()

{

  /// halt playback on all channels
  Mix_HaltChannel(-1);


  /// delete all Sounds
  std::map<unsigned int, SdlSound*>::iterator soundIt    = _sounds.begin();
  std::map<unsigned int, SdlSound*>::iterator soundEndIt = _sounds.end();

  for (; soundIt != soundEndIt; ++soundIt)
  {
    delete (*soundIt).second;
  }

  _sounds.clear();


  /// delete all sound sources
  std::map<unsigned int, SdlSoundSource*>::iterator soundSrcIt    = _soundSources.begin();
  std::map<unsigned int, SdlSoundSource*>::iterator soundSrcEndIt = _soundSources.end();

  for (; soundSrcIt != soundSrcEndIt; ++soundSrcIt)
  {
    delete (*soundSrcIt).second;
  }

  _soundSources.clear();



  /// ...
  _assignMap.clear();



  /// Reset the stack with the unused channels
  while (_unusedChannelsStack.size())
  {
    _unusedChannelsStack.pop();
  }

  for (unsigned int i=0; i != _numMixChannels; ++i)
  {
    _unusedChannelsStack.push(i);
  }


  /// create the default sound source
  _soundSources[GLOOST_SDLSOUNDMANAGER_DEFAULT_SOURCE] = new SdlSoundSource();

}


////////////////////////////////////////////////////////////////////////////////


  /// load a sound and return a id

unsigned int
SdlSoundManager::createSound(const std::string& filePath)
{

  SdlSound* newSound = new SdlSound(filePath);

  if (newSound->getChunk() != 0)
  {
    ++_soundIdCounter;
    _sounds[_soundIdCounter] = newSound;
    return _soundIdCounter;
  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in createSound(const std::string& file_name):" ;
    std::cout << std::endl << "         Could NOT load sound file " << filePath << "!";
    std::cout << std::endl << "         Returning 0 as id here";
    std::cout << std::endl;

    delete newSound;

    return 0;
  }

}


////////////////////////////////////////////////////////////////////////////////


  /// load a sound, assign a name for it and return a id

unsigned int
SdlSoundManager::createSoundAndAssignName(const std::string& filePath, const std::string& name)
{

  unsigned int newId = createSound(filePath);

  assignName(newId, name);

  return newId;

}


////////////////////////////////////////////////////////////////////////////////


  /// Add existing Sound to be managed by the SdlSoundManager (refCount +)

unsigned int
SdlSoundManager::addSound(SdlSound* sound)
{
  ++_soundIdCounter;
  _sounds[_soundIdCounter] = sound;

  sound->takeReference();

  return _soundIdCounter;
}


////////////////////////////////////////////////////////////////////////////////


  /// Add existing Core to be managed by the SdlSoundManager and assign a (unique) name for it (refCount +)

unsigned int
SdlSoundManager::addSoundAndAssignName(SdlSound* sound, const std::string& name)
{
  unsigned int soundId = addSound(sound);

  assignName(soundId, name);

  return soundId;
}


////////////////////////////////////////////////////////////////////////////////


  /// Assign a (unique) name for a core within the core manager (refCount +)

void
SdlSoundManager::assignName(unsigned int soundId, const std::string& name)
{

  if (soundExists(soundId))
  {
    _assignMap[name] = soundId;
  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in SdlSoundManager::assignName(unsigned int coreId, const std::string& name):" ;
    std::cout << std::endl << "         SoundId " << soundId << " was not found in the SdlSoundManager!";
    std::cout << std::endl;
  }

}


////////////////////////////////////////////////////////////////////////////////



  /// check if a SdlSound with id exists within the SdlSoundManager.

bool
SdlSoundManager::soundExists(unsigned int soundId)
{
  std::map<unsigned int, SdlSound*>::iterator pos = _sounds.find(soundId);

  if (pos != _sounds.end())
  {
    return true;
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////


  /// check if a SdlSound with soundName exists within the SdlSoundManager.

bool
SdlSoundManager::soundExists(const std::string& soundName)
{
  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(soundName);

  if (pos != _assignMap.end())
  {
    return true;
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////


  /// create a soundSource, returns a id for it

unsigned int
SdlSoundManager::createSoundSource()
{
  ++_sourceIdCounter;
  _soundSources[_sourceIdCounter] = new SdlSoundSource();
  return _soundIdCounter;
}


////////////////////////////////////////////////////////////////////////////////


  /// returns a pointer a soundSource

SdlSoundSource*
SdlSoundManager::getSoundSource(unsigned int soundSourceId)
{
  if (soundSourceExists(soundSourceId))
  {
    return _soundSources[soundSourceId];
  }

  std::cout << std::endl;
  std::cout << std::endl << "ERROR in SdlSoundManager::getSoundSource(unsigned int soundSourceId):" ;
  std::cout << std::endl << "         Sound source with id " << soundSourceId << " was not found in";
  std::cout << std::endl << "         the SdlSoundManager! Returning source with id 0.!";
  std::cout << std::endl;

  return _soundSources[GLOOST_SDLSOUNDMANAGER_DEFAULT_SOURCE];
}


////////////////////////////////////////////////////////////////////////////////



  /// check if a SdlSound with soundSourceId exists within the SdlSoundManager.

bool
SdlSoundManager::soundSourceExists(unsigned int soundSourceId) const
{
  std::map<unsigned int, SdlSoundSource*>::const_iterator pos = _soundSources.find(soundSourceId);

  if (pos != _soundSources.end())
  {
    return true;
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////
//
//
//  /// returns reference to an existing Core object and increment the reference counter (refCount +)
//
//Sdl*
//SdlSoundManager::getCoreReference(unsigned int id)
//{
//  std::map<unsigned int, Core*>::iterator pos = _cores.find(id);
//
//  if (pos != _cores.end())
//  {
//    /// (refCount +)
//    (*pos).second->takeReference();
//    return (*pos).second;
//  }
//
//  std::cout<< std::endl;
//  std::cout<< std::endl << "ERROR in SdlSoundManager::getCoreReference(unsigned int id)):";
//  std::cout<< std::endl << "         Could not find existing core with id = " << id << " !";
//  std::cout<< std::endl << "         Use";
//  std::cout<< std::endl << "           SdlSoundManager::coreIdExists(unsigned int coreId)";
//  std::cout<< std::endl << "         if you are unsure if a core exists";
//  std::flush(std::cout);
//
//  return 0;
//}
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
//  /// returns reference to an existing Core object and increment the reference counter (refCount +)
//
//Core*
//SdlSoundManager::getCoreReference(const std::string& coreName)
//{
//  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);
//
//  if (pos != _assignMap.end())
//  {
//    unsigned int id = (*pos).second;
//    /// (refCount +)
//    _cores[id]->takeReference();
//    return _cores[id];
//  }
//
//  std::cout<< std::endl;
//  std::cout<< std::endl << "ERROR in SdlSoundManager::getCoreReference(const std::string& coreName)):";
//  std::cout<< std::endl << "         Could not find existing core with name = " << coreName << " !";
//  std::cout<< std::endl << "         Use";
//  std::cout<< std::endl << "           SdlSoundManager::coreIdExists(const std::string& coreName)";
//  std::cout<< std::endl << "         if you are unsure if a core exists";
//  std::flush(std::cout);
//
//  return 0;
//}


////////////////////////////////////////////////////////////////////////////////


  /// returns reference to gloost::Core object WITHOUT incrementing the reference counter

SdlSound*
SdlSoundManager::getSoundWithoutRefcount(unsigned int soundId)
{
  std::map<unsigned int, SdlSound*>::iterator pos = _sounds.find(soundId);

  if (pos != _sounds.end())
  {
    /// (refCount +)
    (*pos).second->takeReference();
    return (*pos).second;
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in SdlSoundManager::getCoreWithoutRefcount(unsigned int id):";
  std::cout<< std::endl << "         Could not find existing sound with id = " << soundId << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           SdlSoundManager::soundExists(unsigned int soundId)";
  std::cout<< std::endl << "         if you are unsure if a sound exists";
  std::flush(std::cout);

  return 0;
}


////////////////////////////////////////////////////////////////////////////////



  ///

unsigned int
SdlSoundManager::getSoundIdForName(const std::string& soundName)
{

  if (soundExists(soundName))
  {
    return _assignMap[soundName];
  }

  return false;
}




////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  /// returns reference to an existing Core object and incrementing the reference counter (refCount +)
//
//Core*
//SdlSoundManager::getCoreWithoutRefcount(const std::string& coreName)
//{
//  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);
//
//  if (pos != _assignMap.end())
//  {
//    unsigned int id = (*pos).second;
//    return _cores[id];
//  }
//
//  std::cout<< std::endl;
//  std::cout<< std::endl << "ERROR in SdlSoundManager::getCoreWithoutRefcount(const std::string& coreName):";
//  std::cout<< std::endl << "         Could not find existing core with name = " << coreName << " !";
//  std::cout<< std::endl << "         Use";
//  std::cout<< std::endl << "           SdlSoundManager::coreIdExists(const std::string& coreName)";
//  std::cout<< std::endl << "         if you are unsure if a core exists";
//  std::flush(std::cout);
//
//  return 0;
//}
//
//
////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  /// drops reference to gloost::Core object (refCount -)
//
//void
//SdlSoundManager::dropReference(unsigned int id)
//{
//  std::map<unsigned int, Core*>::iterator pos = _cores.find(id);
//
//  if (pos != _cores.end())
//  {
//    /// (refCount -)
//    (*pos).second->dropReference();
//  }
//
//  std::cout<< std::endl;
//  std::cout<< std::endl << "Warning from SdlSoundManager::dropReference(unsigned int id):";
//  std::cout<< std::endl << "             Could not find existing core with id = " << id << " !";
//  std::flush(std::cout);
//
//  return;
//}
//

//////////////////////////////////////////////////////////////////////////////////////////
//
//
//  /// drops reference to gloost::Core object (refCount -)
//
//void
//SdlSoundManager::dropReference(const std::string& coreName)
//{
//  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);
//
//  if (pos != _assignMap.end())
//  {
//    unsigned int id = (*pos).second;
//    _cores[id]->dropReference();
//    return;
//  }
//
//  std::cout<< std::endl;
//  std::cout<< std::endl << "ERROR in SdlSoundManager::getCoreWithoutRefcount(const std::string& coreName):";
//  std::cout<< std::endl << "         Could not find existing core with name = " << coreName << " !";
//  std::cout<< std::endl << "         Use";
//  std::cout<< std::endl << "           SdlSoundManager::coreIdExists(const std::string& coreName)";
//  std::cout<< std::endl << "         if you are unsure if a core exists";
//  std::flush(std::cout);
//
//  return;
//}


////////////////////////////////////////////////////////////////////////////////


  /// take a channel from the stack of free channels, returns -1 if no channel was available

int
SdlSoundManager::takeChannel()
{
  if (_unusedChannelsStack.size())
  {
    unsigned int freeChannelNum = _unusedChannelsStack.top();
    _unusedChannelsStack.pop();

//    std::cout<< std::endl << "Message from SdlSoundManager::takeChannel():";
//    std::cout<< std::endl << "             Took channel " << freeChannelNum;
//    std::cout<< std::endl;

    return freeChannelNum;
  }


  std::cout<< std::endl;
  std::cout<< std::endl << "Warning from SdlSoundManager::takeChannel():";
  std::cout<< std::endl << "             Out of free channels. All " << _numMixChannels << " are in use";
  std::cout<< std::endl << "             1. You can try to start the gloost::SdlSoundManager with more MixChannels ";
  std::cout<< std::endl << "             2. Do you call gloost::SdlSoundManager::frameStep() every Frame???";
  std::flush(std::cout);

  return -1;
}


////////////////////////////////////////////////////////////////////////////////


  /// give back a channel to be reused by the sound system

void
SdlSoundManager::dropChannel(unsigned int channelNum)
{

//  std::cout<< std::endl << "Message from SdlSoundManager::dropChannel(unsigned int channelNum):";
//  std::cout<< std::endl << "             Dropped channel " << channelNum;
//  std::cout<< std::endl;

  _unusedChannelsStack.push(channelNum);
}


////////////////////////////////////////////////////////////////////////////////


  /// deletes all unused Cores (... if the reference counter of a core is 0)

void
SdlSoundManager::cleanUp()
{

  std::map<unsigned int, SdlSound*>::iterator soundIt    = _sounds.begin();
  std::map<unsigned int, SdlSound*>::iterator soundEndIt = _sounds.end();

  for (; soundIt != soundEndIt; ++soundIt)
  {

    /// erase and delete the core if it is not referenced. Hopefully the resources
    /// used by the core like shaders, FBOs or UniformSets are referenced from somewhere
    /// else, since they will not be deleted
    if ( !(*soundIt).second->isReferenced())
    {
      unsigned int id = (*soundIt).first;

      delete (*soundIt).second;
      _sounds.erase( soundIt );


      /// remove all entries from name map
      std::map<std::string, unsigned int>::iterator nameIt    = _assignMap.begin();
      std::map<std::string, unsigned int>::iterator nameEndIt = _assignMap.end();

      for (; nameIt != nameEndIt; ++nameIt)
      {
        if ( (*nameIt).second == id   )
        {
          _assignMap.erase(nameIt);
        }
      }


    }

  }



}


//////////////////////////////////////////////////////////////////////////////////////////


  /// returns number of all Cores within the manager

unsigned int
SdlSoundManager::getSize() const
{
  return _sounds.size();
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// prints infos for all textures within the manager

void
SdlSoundManager::printInfo() const
{


}


//////////////////////////////////////////////////////////////////////////////////////////
















///////////////////////////////////////////////////////////////////////////////


/* extern */
std::ostream&
operator<< (std::ostream& os, const SdlSoundManager& tm)
{

  os << std::endl << "SdlSoundManager" << std::endl << "{  nnnnnnooooottt implemented...";
//  os << std::endl << "    size:    " << tm.getSize();
//
//  os << std::endl;
//
//  tm.printTextureInfo();
//
//  os << std::endl;
//
//  os << std::endl << "} // SdlSoundManager" << std::endl;
//  std::flush(os);
//
  return os;
}












} // namespace gloost
