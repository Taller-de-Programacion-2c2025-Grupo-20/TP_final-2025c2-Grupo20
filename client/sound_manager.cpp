#include "sound_manager.h"
#include <SDL2/SDL.h>
#include <iostream>

#ifndef DATA_PATH
#define DATA_PATH "../data"
#endif

SoundManager::SoundManager(SDL2pp::Mixer& mixer)
    : mixer(mixer),
      engineIdle(DATA_PATH "/sounds/1000 RPM.wav"),
      skidChunk(DATA_PATH "/sounds/skid.wav"),
      crashChunk(DATA_PATH "/sounds/crash.wav"),
      bgMusic(DATA_PATH "/sounds/soundtrack.ogg"),
      raceEndSound(DATA_PATH "/sounds/ending.ogg")
{
    std::cout << "SoundManager cargado.\n";
    mixer.SetMusicVolume(MIX_MAX_VOLUME / 2);
}

void SoundManager::updateEngineSound() {
    if (engineChannel == -1) {
        engineChannel = mixer.PlayChannel(-1, engineIdle, -1);
        if (engineChannel < 0) {
            std::cerr << "Error al reproducir motor idle: "
                      << Mix_GetError() << std::endl;
        }
    }
}

void SoundManager::stopEngineSound() {
    if (engineChannel != -1) {
        mixer.HaltChannel(engineChannel);
        engineChannel = -1;
    }
}

void SoundManager::playCrash() {

    Uint32 now = SDL_GetTicks();

    // Cooldown para evitar 10 choques seguidos
    if (now - lastCrashTicks < CRASH_COOLDOWN_MS)
        return;

    lastCrashTicks = now;

    // No reproducir si ya está sonando un crash reciente
    if (crashChannel >= 0 && mixer.IsChannelPlaying(crashChannel))
        return;

    crashChannel = mixer.PlayChannel(-1, crashChunk, 0);

    if (crashChannel < 0) {
        std::cerr << "Error al reproducir crash: "
                  << Mix_GetError() << std::endl;
    }
}

void SoundManager::updateBackgroundMusic(double elapsedSeconds) {

    if (!bgMusicStarted && elapsedSeconds >= 6.0) {
        try {
            mixer.PlayMusic(bgMusic, -1);
            bgMusicStarted = true;
        } catch (const SDL2pp::Exception& e) {
            std::cerr << "Error al reproducir música de fondo: "
                      << e.GetSDLError() << std::endl;
        }
    }
}


void SoundManager::playRaceEnd() {
    stopBackgroundMusic();

    int ch = mixer.PlayChannel(-1, raceEndSound, 0);
    if (ch < 0) {
        std::cerr << "Error al reproducir ending: "
                  << Mix_GetError() << std::endl;
    }
}

void SoundManager::stopBackgroundMusic() {
    mixer.HaltMusic();
    bgMusicStarted = false;
}


void SoundManager::updateSkidSound(bool braking, float speed_kmh) {
    const float MIN_SKID_SPEED = 40.0f; // umbral para que suene

    bool shouldSkid = braking && (speed_kmh > MIN_SKID_SPEED);

    if (shouldSkid) {
        // Si no está sonando, arrancar el loop
        if (skidChannel == -1 || !mixer.IsChannelPlaying(skidChannel)) {
            skidChannel = mixer.PlayChannel(-1, skidChunk, -1); // loop infinito
            if (skidChannel < 0) {
                std::cerr << "Error al reproducir skid: "
                          << Mix_GetError() << std::endl;
            }
        }
    } else {
        // Si no debería sonar, detenerlo
        if (skidChannel != -1) {
            mixer.HaltChannel(skidChannel);
            skidChannel = -1;
        }
    }
}

void SoundManager::stopSkid() {
    if (skidChannel != -1) {
        mixer.HaltChannel(skidChannel);
        skidChannel = -1;
    }
}