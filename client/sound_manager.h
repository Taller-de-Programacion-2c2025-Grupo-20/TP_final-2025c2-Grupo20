#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>  
#include <SDL2pp/Exception.hh>

class SoundManager {
public:
    explicit SoundManager(SDL2pp::Mixer& mixer);

    void updateEngineSound();
    void stopEngineSound();

    void playSkid();
    void playCrash();
    void updateBackgroundMusic(double elapsedSeconds);

    void playRaceEnd();
    void stopBackgroundMusic();

private:
    SDL2pp::Mixer& mixer;

    // Motor
    int engineChannel = -1;

    // Crash control
    int  crashChannel = -1;
    Uint32 lastCrashTicks = 0;

    // Sonidos
    SDL2pp::Chunk engineIdle;
    SDL2pp::Chunk skidChunk;
    SDL2pp::Chunk crashChunk;

    // Cooldown mínimo entre golpes
    static constexpr Uint32 CRASH_COOLDOWN_MS = 250;

    SDL2pp::Music bgMusic;
    bool bgMusicStarted = false;

    SDL2pp::Chunk raceEndSound;
};

#endif
