#pragma once

#include "SpriteAtlasManager.h"


class Texture;
class UnitAnimator
{
public:
    UnitAnimator(Atlas* atlas,Texture* texture);

    void Play(const std::string actionName);
    void Update(float elapsedTime);
    void DrawShade(std::initializer_list<std::pair<Color4f,Color4f>> colorPairs);
    bool IsDone() const;

private:
    Atlas* atlas;
    Texture* texture;

    AnimationClip* current = nullptr;
    int frameIndex = 0;
    float elapsed = 0.0f;
    bool isDonePlaying = false;
};
