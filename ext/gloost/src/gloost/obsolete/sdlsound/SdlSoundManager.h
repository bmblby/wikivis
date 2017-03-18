
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



#ifndef GLOOST_SDL_SOUND_MANAGER
#define GLOOST_SDL_SOUND_MANAGER


/// gloost includes
#include <gloost/sdlsound/SdlSound.h>
#include <gloost/sdlsound/SdlSoundSource.h>

/// cpp includes
#include <map>
#include <stack>



#define GLOOST_SDLSOUNDMANAGER_DEFAULT_SOURCE 0





namespace gloost
{

  /// SdlSound container with refcount abilities

class SdlSoundManager
{


  public:

    virtual ~SdlSoundManager();


    /// get an instance to the CoreManager
    static SdlSoundManager* const getInstance();

    /// copy contructor to avoid assignment to a non reference
    explicit SdlSoundManager(SdlSoundManager const&) {}


    /// configures and starts sdl_mixer (call ths after SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) | ...)
    bool start( unsigned int numMixChannels = 16,
                unsigned int sampleRate     = 44100,
                Uint16       audioFormat    = AUDIO_S16SYS,
                unsigned int numChannels    = 2,
                unsigned int sampleBuffer   = 2048);



    /// close the soundmanager, delete all sound resources
    void reset();



    /// call this function every frame
    void frameStep();



    /// load a sound and return a id
    unsigned int createSound(const std::string& filePath);
    /// load a sound, assign a name for it and return a id
    unsigned int createSoundAndAssignName(const std::string& filePath, const std::string& name);


    /// Add existing Sound to be managed by the SdlSoundManager (refCount +)
    unsigned int addSound(SdlSound* sound);

    /// Add existing Sound to be managed by the SdlSoundManager and assign a (unique) name for it (refCount +)
    unsigned int addSoundAndAssignName(SdlSound* sound, const std::string& name);

    /// Assign a (please unique) name for a core within the SdlSoundManager (refCount +)
    void assignName(unsigned int coreId, const std::string& name);



    /// check if a Sound with id exists within the SdlSoundManager
    bool soundExists(unsigned int soundId);

    /// check if a Sound with id exists within the SdlSoundManager
    bool soundExists(const std::string& soundName);



//    /// returns reference to an existing Core object and increment the reference counter (refCount +)
//    Core* getSoundReference(unsigned int coreId);
//
//    /// returns reference to an existing Core object and increment the reference counter (refCount +)
//    Core* getSoundReference(const std::string& coreName);
//
    /// returns reference to gloost::SdlSound object WITHOUT incrementing the reference counter
    SdlSound* getSoundWithoutRefcount(unsigned int soundId);

    /// returns reference to gloost::SdlSound object WITHOUT incrementing the reference counter
    SdlSound* getSoundWithoutRefcount(const std::string& soundName);


    ///
    unsigned int getSoundIdForName(const std::string& soundName);


//    /// drops reference to gloost::Core object (refCount -)
//    void dropReference(unsigned int coreId);
//
//    /// drops reference to gloost::Core object (refCount -)
//    void dropReference(const std::string& coreName);



    /// create a soundSource, returns a id for it
    unsigned int createSoundSource();

    /// returns a pointer a soundSource
    SdlSoundSource* getSoundSource(unsigned int soundSourceId = 0);

    /// check if a Sound with id exists within the SdlSoundManager
    bool soundSourceExists(unsigned int soundSourceId) const;



    /// take a channel from the stack of free channels, returns -1 if no channel was available
    int takeChannel();

    /// give back a channel to be reused by the sound system
    void dropChannel(unsigned int channelNum);



    /// unload all unused SdlSound s (... if the reference counter of a core is 0)
    void cleanUp();


    /// returns number of all Cores within the manager
    unsigned int getSize() const;



    /// prints info about the manager
    void printInfo() const;


  protected:

    ///
    static SdlSoundManager* _theinstance;



    /// internal ID counter for unique IDs
    unsigned int _soundIdCounter;

    /// map to hold the cores
    std::map<unsigned int, SdlSound*> _sounds;



    /// map to hold name->id assoziations to share textures
    std::map<std::string, unsigned int> _assignMap;



    /// stack with channel numbers
    std::stack<unsigned int> _unusedChannelsStack;



    /// map to hold the sound sources
    std::map<unsigned int, SdlSoundSource*> _soundSources;

    /// internal ID counter for unique IDs
    unsigned int _sourceIdCounter;



    /// configuration of the audio device
    unsigned int _numMixChannels;  /// Hardware channels: 8, 16, 24, 32, 64, ...
    unsigned int _sampleRate;      /// 11025, 22050, 44100
    Uint16       _audioFormat;     /// AUDIO_S16SYS, ...
    unsigned int _numChannels;     /// 1 = mono, 2 = stereo, ...
    unsigned int _sampleBuffer;    /// 1024, 2048, 4096, ... (!!!latency)




    /// class constructor
    SdlSoundManager();

};


// ostream operator
extern std::ostream& operator<< (std::ostream&, const SdlSoundManager& );



}  // namespace gloost



#endif // GLOOST_SDL_SOUND_MANAGER
