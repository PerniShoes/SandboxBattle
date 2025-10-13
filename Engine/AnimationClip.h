#pragma once
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4820)  // disable padding warning

struct AnimationClip
{
    std::vector<std::string> frameNames; // ordered list of atlas frame keys
    std::string name;
    float frameTime = 0.020f;            // seconds per frame // Don't adjust here 
    bool loop = true;
};
#pragma warning(pop)  // restore previous warning state