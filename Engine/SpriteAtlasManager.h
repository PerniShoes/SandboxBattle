#pragma once
#include <tinyxml2.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include "utils.h"

struct FrameData
{
    // sscanf_s requires ints to initilize data properly
    int x = 0,y = 0,w = 0,h = 0;
    Rectf ToRectf() const
    {
        return Rectf{float(x), float(y), float(w), float(h)};
    }
};

struct Atlas
{
    std::string name;                        // base name (without extension)
    std::string pngPath;                     // path to the texture
    std::unordered_map<std::string,FrameData> frames;
};

class SpriteAtlasManager
{
public:
    // Load all atlases from a folder
    void LoadFolder(const std::string& folderPath);

    // Get atlas by name
    const Atlas* GetAtlas(const std::string& name) const;

public:
    bool LoadPlist(const std::string& plistPath,
        std::unordered_map<std::string,FrameData>& outFrames);

    std::vector<Atlas> m_Atlases;
};

