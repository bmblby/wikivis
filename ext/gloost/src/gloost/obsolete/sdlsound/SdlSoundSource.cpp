
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
#include <gloost/sdlsound/SdlSoundSource.h>
#include <gloost/sdlsound/SdlSoundJob.h>
#include <gloost/sdlsound/SdlSoundManager.h>



/// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

////////////////////////////////////////////////////////////////////////////////


  /// class constructor

SdlSoundSource::SdlSoundSource():
  _soundManager(SdlSoundManager::getInstance()),
  _soundJobIdCounter(0),
  _runningSoundJobs(),
  _sourcePosition(),
  _sourceVolume(1.0),
  _sourcePanning(0.0),
  _frameCounter(0)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


  /// class destructor

SdlSoundSource::~SdlSoundSource()
{
	_runningSoundJobs.clear();
}


////////////////////////////////////////////////////////////////////////////////


  /// this function is called every frame from the SdlSoundManager

void
SdlSoundSource::frameStep()
{



  ++_frameCounter;

//  if (_frameCounter%30 == 0)
  {

    /// handle all running sound jobs
    std::map<unsigned int, SdlSoundJob*>::iterator jobsIt    = _runningSoundJobs.begin();
    std::map<unsigned int, SdlSoundJob*>::iterator jobsEndIt = _runningSoundJobs.end();

    for (; jobsIt != jobsEndIt; ++jobsIt)
    {
      if (Mix_Playing((*jobsIt).second->_channel)) //|| !(*jobsIt).second->_halted)
      {

        /// regulate volume
        Mix_Volume((*jobsIt).second->_channel, (int)((*jobsIt).second->_volume*_sourceVolume*128.0));


        /// regulate panning
        float volumeRight = (unsigned char)((_sourcePanning+1.0f)/2.0f*255.0f);
        Mix_SetPanning((*jobsIt).second->_channel, 255-volumeRight, (unsigned char)(volumeRight));

      }
      else
      {

        Mix_HaltChannel((*jobsIt).second->_channel);

        /// sound is not playing: kill the sound job and free the channel
        unsigned int channel = (*jobsIt).second->_channel;
        delete (*jobsIt).second;
        _runningSoundJobs.erase(jobsIt);
        _soundManager->dropChannel(channel);
      }
    }

  }


}


////////////////////////////////////////////////////////////////////////////////


  /// check if a Sound job with soundJobId exists within the SdlSoundSource

bool
SdlSoundSource::soundJobExists(unsigned int soundJobId)
{
  std::map<unsigned int, SdlSoundJob*>::iterator pos = _runningSoundJobs.find(soundJobId);

  if (pos != _runningSoundJobs.end())
  {
    return true;
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////


  /// get a running SdlSoundJob (be shure the job exists with soundJobExists(unsigned int soundJobId) )

SdlSoundJob*
SdlSoundSource::getSoundJob(unsigned int soundJobId)
{
  std::map<unsigned int, SdlSoundJob*>::iterator pos = _runningSoundJobs.find(soundJobId);

  if (pos != _runningSoundJobs.end())
  {
    return (*pos).second;
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in SdlSoundSource::SdlSoundJob* getSoundJob(unsigned int soundJobId):";
  std::cout<< std::endl << "         Could not find existing SdlSoundJob with id = " << soundJobId << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           SdlSoundSource::soundJobExists(unsigned int soundJobId)";
  std::cout<< std::endl << "         if you are unsure if a sound job exists";
  std::flush(std::cout);

  return false;
}


////////////////////////////////////////////////////////////////////////////////


  /// starts playing a sound and returns a soundJobId for this play

unsigned int
SdlSoundSource::play(unsigned int soundId, float volume, unsigned int loops)
{

  /// check if the sound exists
  if (!_soundManager->soundExists(soundId))
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in SdlSoundSource::play(unsigned int soundId, unsigned int volume, unsigned int loops)" ;
    std::cout << std::endl << "         Sound with id " << soundId << " does NOT exist within the SoundManager!";
    std::cout << std::endl;
    return 0;
  }


  /// get a free channel (throws a messege if no channel is available)
  int freeChannel = _soundManager->takeChannel();

  /// maybe there was no channel left?
  if (freeChannel == -1)
  {
    return 0;
  }




  /// create a new soundjob
  SdlSoundJob* newJob = new SdlSoundJob( _soundJobIdCounter,
                                          soundId,
                                          volume,
                                          loops,
                                          freeChannel);



  /// create a new soundjob id
  ++_soundJobIdCounter;

  _runningSoundJobs[_soundJobIdCounter] = newJob;

  _soundManager->getSoundWithoutRefcount(soundId)->play(volume*_sourceVolume, loops, freeChannel);

  return _soundJobIdCounter;
}


////////////////////////////////////////////////////////////////////////////////


  /// ...

unsigned int
SdlSoundSource::play(const std::string& soundName, float volume, unsigned int loops)
{
 /// check if the sound exists
  if (!_soundManager->soundExists(soundName))
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in SdlSoundSource::play(const std::string& soundName, unsigned int volume, unsigned int loopss)" ;
    std::cout << std::endl << "         Sound with name " << soundName << " does NOT exist within the SoundManager!";
    std::cout << std::endl;
    return 0;
  }


  /// get a free channel (throws a messege if no channel is available)
  int freeChannel = _soundManager->takeChannel();

  /// maybe there was no channel left?
  if (freeChannel == -1)
  {
    return 0;
  }


  unsigned int soundId = _soundManager->getSoundIdForName(soundName);



  /// create a new soundjob
  SdlSoundJob* newJob = new SdlSoundJob( _soundJobIdCounter,
                                          soundId,
                                          volume,
                                          loops,
                                          freeChannel);



  /// create a new soundjob id
  ++_soundJobIdCounter;

  _runningSoundJobs[_soundJobIdCounter] = newJob;

  _soundManager->getSoundWithoutRefcount(soundId)->play(volume*_sourceVolume, loops, freeChannel);

  return _soundJobIdCounter;
}


////////////////////////////////////////////////////////////////////////////////


  /// get postion of the sound source in the world


const Point3&
SdlSoundSource::getPosition() const
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


  /// set postion of the sound source in the world

void
SdlSoundSource::setPosition(const Point3& position)
{





//	int Mix_SetPosition(int channel, Sint16 angle, Uint8 distance)
}


////////////////////////////////////////////////////////////////////////////////


  /// ...

const float&
SdlSoundSource::getVolume() const
{
	return _sourceVolume;
}


////////////////////////////////////////////////////////////////////////////////


  /// ...

void
SdlSoundSource::setVolume(const float& volume)
{
	_sourceVolume = volume;
}


////////////////////////////////////////////////////////////////////////////////


  /// -1 = left, 0 = center, 1 = right

const float&
SdlSoundSource::getPanning() const
{
	return _sourcePanning;
}


////////////////////////////////////////////////////////////////////////////////


  /// -1 = left, 0 = center, 1 = right

void
SdlSoundSource::setPanning(const float& panning)
{
	_sourcePanning = panning;
}


////////////////////////////////////////////////////////////////////////////////


  /// ...

bool
SdlSoundSource::isPlaying(unsigned int soundId) const
{

}


////////////////////////////////////////////////////////////////////////////////










////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


