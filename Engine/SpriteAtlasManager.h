#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include "utils.h"
#include "AnimationClip.h"

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
    std::unordered_map<std::string,AnimationClip> animations;
};

class SpriteAtlasManager
{
public:
    // Load all atlases from a folder
    void LoadFolder(const std::string& folderPath);

    // Get atlas by name
    Atlas* GetAtlas(const std::string& name);
    // SpriteAtlasManager.cpp (inside your manager)
    void BuildAnimations(Atlas& atlas);
    

public:
    bool LoadPlist(const std::string& plistPath,
        std::unordered_map<std::string,FrameData>& outFrames);

    std::vector<Atlas> m_Atlases;
};

