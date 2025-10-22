#include "AudioManager.h"

void AudioManager::LoadFolder(const std::string& folderPath)
{
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file()) continue;

        std::string path = entry.path().string();
        std::string ext = entry.path().extension().string();

        if (ext == ".m4a")
        {
            std::string base = entry.path().stem().string();
            std::string pngPath = folderPath + "/" + base + ".m4a";

            AudioFile file;
            file.name = base;
            file.audioPath = pngPath;

            /* if (LoadPlist(path)
             {
                 //m_AudioFiles.push_back(file);
                 std::cout << "Loaded audio: " << base << "\n";
             }*/
        }
    }
}


bool AudioManager::LoadAudio(const std::string& audioPath)
{
 
    return true;
}