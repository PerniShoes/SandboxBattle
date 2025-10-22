#pragma once

// Disable warnings for SDL
#pragma warning(push, 0) // All warnings
//#pragma warning(disable: 5262) // implicit fall-through warning
#include "SDL_mixer.h"
#pragma warning(pop)

#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include "SoundStream.h"
#include "SoundEffect.h"

#include <iostream>

struct SoundPack
{
    std::unordered_map<std::string, SoundEffect> sfx; // Name of file and it's SoundEffect object
    float defaultVolume = 0.50f; // 50%
    int loops = 1;

};

class AudioManager
{
    public:

        void LoadMusic(const std::string& folderPath);
        void LoadSounds(const std::string& folderPath);

        std::string NormalizePackName(const std::string& name);
        const SoundPack& GetSoundPack(const std::string& nameOfUnit);
        const std::unordered_map<std::string,SoundStream>& GetMusicPack();
        // 0-128
        void SetDefaultVolumeMusic(int volume);
        void SetDefaultVolumeSounds(int volume);
    private:
        std::string DeterminePackName(const std::string& filename);
        std::string GetSuffix(const std::string& filename,const std::string& prefix);
        std::string GetMusicKeyName(const std::filesystem::path& filePath);

        std::unordered_map<std::string, SoundPack> m_Sounds;  // Name of unit
        std::unordered_map<std::string, SoundStream> m_Music; // Name of file

};

