#include "SpriteAtlasManager.h"
#include <tinyxml2.h>

void SpriteAtlasManager::LoadFolder(const std::string& folderPath)
{
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file()) continue;

        std::string path = entry.path().string();
        std::string ext = entry.path().extension().string();

        if (ext == ".plist")
        {
            std::string base = entry.path().stem().string();
            std::string pngPath = folderPath + "/" + base + ".png";

            Atlas atlas;
            atlas.name = base;
            atlas.pngPath = pngPath;
            if (LoadPlist(path,atlas.frames))
            {
                m_Atlases.push_back(atlas);
                std::cout << "Loaded atlas: " << base << "\n";
            }
        }
    }
}

Atlas* SpriteAtlasManager::GetAtlas(const std::string& name)
{
    for (auto& atlas : m_Atlases)
    {
        if (atlas.name == name)
        {
            if (atlas.animations.empty())
            {
                BuildAnimations(atlas);
            }
            return &atlas;
        }
    }
    return nullptr;
}


bool SpriteAtlasManager::LoadPlist(const std::string& plistPath,
    std::unordered_map<std::string,FrameData>& outFrames)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(plistPath.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Failed to load plist: " << plistPath << "\n";
        return false;
    }

    tinyxml2::XMLElement* rootDict = doc.FirstChildElement("plist")->FirstChildElement("dict");
    if (!rootDict) return false;

    // Find the "frames" key in the root dict
    tinyxml2::XMLElement* keyElem = rootDict->FirstChildElement("key");
    while (keyElem)
    {
        if (std::string(keyElem->GetText()) == "frames") break;
        keyElem = keyElem->NextSiblingElement("key");
    }
    if (!keyElem)
    {
        std::cerr << "No 'frames' key found in plist: " << plistPath << "\n";
        return false;
    }

    // The value of "frames" is another dict containing all actual frames
    tinyxml2::XMLElement* framesDict = keyElem->NextSiblingElement("dict");
    if (!framesDict) return false;

    // Iterate over each frame in framesDict
    for (tinyxml2::XMLElement* frameKey = framesDict->FirstChildElement("key");
        frameKey; frameKey = frameKey->NextSiblingElement("key"))
    {
        std::string frameName = frameKey->GetText();
        tinyxml2::XMLElement* frameDict = frameKey->NextSiblingElement("dict");
        if (!frameDict) continue;

        FrameData f{};
        tinyxml2::XMLElement* elem = frameDict->FirstChildElement("key");
        while (elem)
        {
            std::string tag = elem->GetText();
            tinyxml2::XMLElement* valElem = elem->NextSiblingElement("string");
            if (tag == "frame" && valElem)
            {
                const char* s = valElem->GetText();
                sscanf_s(s,"{{%d,%d},{%d,%d}}",&f.x,&f.y,&f.w,&f.h);

                // Duelyst files use 1 pixel padding. Also sprites in plist files are assuming that I am drawing top to bottom
                // while I am drawing from bottom to top so I have to add height to compensate
                f.y = f.y + f.h + 1; 
            }
            elem = elem->NextSiblingElement("key");
        }

        outFrames[frameName] = f;
    }

    return true;
}

void SpriteAtlasManager::BuildAnimations(Atlas& atlas)
{
    // bucket frames by animation name (without atlas prefix)
    std::unordered_map<std::string,std::vector<std::pair<int,std::string>>> buckets;

    for (auto& [name,f] : atlas.frames)
    {
        // strip extension
        auto dot = name.find_last_of('.');
        std::string base = (dot != std::string::npos) ? name.substr(0,dot) : name;

        // split number suffix
        auto us = base.find_last_of('_');
        std::string animFull = (us != std::string::npos) ? base.substr(0,us) : base;
        std::string numStr = (us != std::string::npos) ? base.substr(us + 1) : "0";

        int index = 0;
        try { index = std::stoi(numStr); }
        catch (...) { index = 0; }

        // Remove all text before last "_" (So sprites are grouped by "action" and not by "entire_name_action")
        std::string anim = animFull;
        auto lastUnderscore = animFull.find_last_of('_');
        if (lastUnderscore != std::string::npos)
        {
            anim = animFull.substr(lastUnderscore + 1);
        }

        buckets[anim].push_back({index, name});
    }

    // sort and build clips
    for (auto& [anim,vec] : buckets)
    {
        std::sort(vec.begin(),vec.end(),
            [](auto& a,auto& b) { return a.first < b.first; });

        AnimationClip clip;
        clip.name = anim;

        for (auto& [idx,fname] : vec)
            clip.frameNames.push_back(fname);

        // mark death animations as non-looping
        if (clip.name.find("death") != std::string::npos)
        {
            clip.loop = false;
        }

        atlas.animations[anim] = std::move(clip);
    }
}

