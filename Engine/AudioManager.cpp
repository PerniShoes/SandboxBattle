#include "AudioManager.h"

std::string AudioManager::GetMusicKeyName(const std::filesystem::path& filePath)
{
    std::string name = filePath.stem().string();

    const std::string prefix = "music_";
    if (name.rfind(prefix,0) == 0) 
    {
        name = name.substr(prefix.size());
    }
    return name;
}

void AudioManager::LoadMusic(const std::string& folderPath)
{
    namespace fs = std::filesystem;

    if (!fs::exists(folderPath))
    {
        std::cerr << "[AudioManager] Folder does not exist: " << folderPath << std::endl;
        return;
    }

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        const auto& path = entry.path();
        std::string ext = path.extension().string();

        // Only reads wav files
        if (ext == ".wav")
        {
            std::string key = GetMusicKeyName(path); // Cuts off "music_" prefix
            std::string fullPath = path.stem().string();

            try
            {
                m_Music.try_emplace(key,path.string());
                std::cout << "[AudioManager] Loaded music: " << key << " (" << fullPath << ")\n";
            }
            catch (const std::exception& e)
            {
                std::cerr << "[AudioManager] Failed to load " << fullPath << ": " << e.what() << std::endl;
            }
        }
    }
}

const std::unordered_map<std::string,SoundStream>& AudioManager::GetMusicPack()
{
    return m_Music;
}

void AudioManager::SetDefaultVolumeMusic(int volume)
{
    for (auto& music : m_Music)
    {
        music.second.SetVolume(volume);
    }

}

std::string AudioManager::GetSuffix(const std::string& filename,const std::string& prefix)
{
    if (filename.rfind(prefix,0) == 0)
    {
        std::string suffix = filename.substr(prefix.size());
        // Remove leading underscores
        if (!suffix.empty() && suffix[0] == '_')
            suffix = suffix.substr(1);
        return suffix;
    }
    return {};
}

// Some sfx names lack underscore, I add it manually to SoundPack keys so I can just do: "sfx_ + unitname + action" inside unit class
std::string AudioManager::NormalizePackName(const std::string& name)
{
    std::string normalized = name;

    if (normalized.size() > 2 && normalized[0] == 'f' && std::isdigit(normalized[1]) && normalized[2] != '_')
    {
        normalized.insert(2,"_");
    }
    return normalized;
}

std::string AudioManager::DeterminePackName(const std::string& filename)
{
    if (filename.rfind("sfx_announcer_",0) == 0)
        return "announcer";

    if (filename.rfind("sfx_artifact_",0) == 0)
        return "artifact";

    if (filename.rfind("sfx_f",0) == 0)
    {
        // Check f1 to f6
        for (int i = 1; i <= 6; ++i)
        {
            std::string prefix1 = "sfx_f" + std::to_string(i) + "_";  // sfx_f1_
            std::string prefix2 = "sfx_f" + std::to_string(i);        // sfx_f1

            if (filename.rfind(prefix1,0) == 0)
            {
                std::string remainder = GetSuffix(filename,prefix1); // Get everything after sfx_f1_
                auto pos = remainder.find('_');
                std::string unitName = (pos == std::string::npos) ? remainder : remainder.substr(0,pos);
                return NormalizePackName("f" + std::to_string(i) + "_" + unitName);
            }

            if (filename.rfind(prefix2,0) == 0)
            {
                std::string remainder = GetSuffix(filename,prefix2); // After sfx_f1
                auto pos = remainder.find('_');
                std::string unitName = (pos == std::string::npos) ? remainder : remainder.substr(0,pos);
                return NormalizePackName("f" + std::to_string(i) + "_" + unitName);
            }
        }
    }

    if (filename.rfind("sfx_neutral_",0) == 0)
    {
        std::string remainder = GetSuffix(filename,"sfx_neutral_"); // Get everything after sfx_neutral_
        auto pos = remainder.find('_');
        std::string unitName = (pos == std::string::npos) ? remainder : remainder.substr(0,pos);
        return "neutral_" + unitName;
    }

    if (filename.rfind("sfx_unit_",0) == 0)
        return "unit";

    // Ignore other files for now (spells, ui, some others)
    return {};
}

void AudioManager::LoadSounds(const std::string& folderPath)
{
    namespace fs = std::filesystem;

    if (!fs::exists(folderPath))
    {
        std::cerr << "[AudioManager] Folder does not exist: " << folderPath << std::endl;
        return;
    }

    std::unordered_map<std::string,SoundPack> packs;

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        const auto& path = entry.path();
        if (path.extension() != ".wav")
            continue;

        std::string filename = path.stem().string();
        std::string packName = DeterminePackName(filename);
        if (packName.empty())
            continue; // ignore files not matching rules

        try
        {
            SoundEffect sfx(path.string());

            packs[packName].sfx.emplace(filename,path.string());

            std::cout << "[AudioManager] Loaded sound: " << filename << " -> pack: " << packName << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "[AudioManager] Failed to load " << filename << ": " << e.what() << "\n";
        }
    }

    m_Sounds = std::move(packs);

}


void AudioManager::SetDefaultVolumeSounds(int volume)
{
    for (auto& sounds : m_Sounds)
    {
        for (auto& currentSfx : sounds.second.sfx)
        {
            currentSfx.second.SetVolume(volume);
        }
    }
}


const SoundPack& AudioManager::GetSoundPack(const std::string& nameOfUnit)
{
    auto it = m_Sounds.find(nameOfUnit);

    if (it != m_Sounds.end())
        return it->second;

    // Default pack (a lot of units have no sfx)
    return m_Sounds.at("neutral_grimrock");
}
