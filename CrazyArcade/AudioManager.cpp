#include "AudioManager.h"
#include <windows.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <string>

namespace {
    FMOD::System* audioSystem = nullptr;
    FMOD::Channel* musicChannel = nullptr;
    std::map<std::string, FMOD::Sound*> sounds;
    std::string soundDirectory;

    bool Check(FMOD_RESULT result) {
        if (result == FMOD_OK) return true;
        OutputDebugStringA(FMOD_ErrorString(result));
        OutputDebugStringA("\n");
        return false;
    }

    FMOD::Sound* Load(const char* filename, bool loop) {
        if (!audioSystem) return nullptr;
        std::string key = std::string(loop ? "music:" : "effect:") + filename;
        auto found = sounds.find(key);
        if (found != sounds.end()) return found->second;
        FMOD::Sound* sound = nullptr;
        FMOD_MODE mode = loop ? (FMOD_CREATESTREAM | FMOD_LOOP_NORMAL) : FMOD_LOOP_OFF;
        if (!Check(audioSystem->createSound((soundDirectory + filename).c_str(), mode, nullptr, &sound)))
            return nullptr;
        sounds[key] = sound;
        return sound;
    }
}

bool Audio::Initialize() {
    if (audioSystem) return true;
    wchar_t path[32768];
    DWORD length = GetModuleFileNameW(nullptr, path, 32768);
    if (!length || length >= 32768) return false;
    std::wstring directory(path, length);
    directory = directory.substr(0, directory.find_last_of(L"\\/") + 1) + L"Resource\\Sound\\";
    int bytes = WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (!bytes) return false;
    std::string utf8(bytes, '\0');
    WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, &utf8[0], bytes, nullptr, nullptr);
    soundDirectory = utf8.c_str();
    if (!Check(FMOD::System_Create(&audioSystem))) return false;
    // Each effect gets a free channel; up to 64 sounds can play together.
    if (!Check(audioSystem->init(64, FMOD_INIT_NORMAL, nullptr))) {
        Shutdown();
        return false;
    }
    return true;
}

void Audio::PlayEffect(const char* filename) {
    FMOD::Sound* sound = Load(filename, false);
    if (sound) Check(audioSystem->playSound(sound, nullptr, false, nullptr));
}

void Audio::PlayMusic(const char* filename) {
    FMOD::Sound* sound = Load(filename, true);
    if (!sound) return;
    StopMusic();
    if (Check(audioSystem->playSound(sound, nullptr, true, &musicChannel))) {
        musicChannel->setPriority(0); // Keep music when effect channels are busy.
        musicChannel->setPaused(false);
    }
}

void Audio::StopMusic() {
    if (musicChannel) musicChannel->stop();
    musicChannel = nullptr;
}

void Audio::Update() {
    if (audioSystem) Check(audioSystem->update());
}

void Audio::Shutdown() {
    StopMusic();
    if (audioSystem) {
        FMOD::ChannelGroup* master = nullptr;
        if (audioSystem->getMasterChannelGroup(&master) == FMOD_OK) master->stop();
        for (auto& entry : sounds) entry.second->release();
        sounds.clear();
        audioSystem->close();
        audioSystem->release();
        audioSystem = nullptr;
    }
}
