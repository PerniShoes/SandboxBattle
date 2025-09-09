#pragma once

#include "SpriteAtlasManager.h"


class Texture;
class UnitAnimator
{
public:
    UnitAnimator(Atlas* atlas,Texture* texture);

    void Play(const AnimationClip& data);
    void Update(float elapsedTime);
    void DrawShade(std::initializer_list<std::pair<Color4f,Color4f>> colorPairs);
    bool IsPlaying() const;

private:
    Atlas* m_atlas;
    Texture* m_texture;

    AnimationClip* m_current = nullptr;
    int m_frameIndex = 0;
    float m_elapsed = 0.0f;
};
