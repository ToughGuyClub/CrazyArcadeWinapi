#pragma once

namespace Audio {
    bool Initialize();
    void PlayEffect(const char* filename);
    void PlayMusic(const char* filename);
    void StopMusic();
    void Update();
    void Shutdown();
}
