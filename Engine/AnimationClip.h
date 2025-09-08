#pragma once
#include <string>
#include <vector>

struct AnimationClip
{
    std::string name;
    std::vector<std::string> frameNames; // ordered list of atlas frame keys
    float frameTime = 0.1f;              // seconds per frame
    bool loop = true;
};
