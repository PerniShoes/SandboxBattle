#pragma once
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
        // Load all audio (m4a) from a folder
        void LoadFolder(const std::string& folderPath);

        SoundPack* GetSoundPack(const std::string& nameOfUnit);
        std::unordered_map<std::string,SoundStream> GetMusicPack();
        // void BuildAnimations(Atlas& atlas);
        bool LoadAudio(const std::string& audioPath);


    private:

        std::unordered_map<std::string, SoundPack> m_AudioFiles; // Name of unit
        std::unordered_map<std::string, SoundStream> m_Music;    // Name of file

};

