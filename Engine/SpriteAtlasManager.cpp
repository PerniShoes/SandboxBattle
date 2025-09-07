#include "SpriteAtlasManager.h"

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

const Atlas* SpriteAtlasManager::GetAtlas(const std::string& name) const
{
    for (const auto& a : m_Atlases)
        if (a.name == name) return &a;
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
            }
            elem = elem->NextSiblingElement("key");
        }

        outFrames[frameName] = f;
    }

    return true;
}
