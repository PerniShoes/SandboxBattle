#pragma once
#include "Texture.h"
#include "SpriteAtlasManager.h"
#include "utils.h"
#include <memory>
#include <string>

class Texture;
class UnitAnimator final
{
public:
    UnitAnimator(Atlas* atlas,std::unique_ptr<Texture> texture);
    UnitAnimator(const UnitAnimator& other) = delete;
    UnitAnimator& operator=(const UnitAnimator& other) = delete;
    UnitAnimator(UnitAnimator&& other) noexcept = default;
    UnitAnimator& operator=(UnitAnimator&& other) noexcept = default;
    ~UnitAnimator() = default;

    void Play(const std::string actionName);
    void Update(float elapsedTime);
    void DrawShade(std::initializer_list<std::pair<Color4f,Color4f>> colorPairs = {});
    bool IsDone() const;

    float GetSpriteWidth();
    std::string GetCurrentAnimation();
    void SetFrameTime(float frameTimeTarget);

private:
    Atlas* atlas;
    std::unique_ptr<Texture> texture;
    std::string currentAnimPlaying;
   
    AnimationClip* current = nullptr;
    int frameIndex = 0;
    float elapsed = 0.0f;
    bool isDonePlaying = false;
    float generalFrameTime = 0.0f;
};
