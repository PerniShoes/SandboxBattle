#include "UnitAnimator.h"

UnitAnimator::UnitAnimator(Atlas* atlas,std::unique_ptr<Texture> texture)
    :atlas{atlas},texture{std::move(texture)}
{
    // Set any frame as deafult (since all frames have same sizes for one unit
    current = &(atlas->animations.find("breathing")->second);
    currentAnimPlaying = "breathing";
}

void UnitAnimator::Play(const std::string actionName)
{
    auto it = atlas->animations.find(actionName);
    if (it == atlas->animations.end())
    {
        std::cerr << "Anim not found: " << actionName << " Name of atlas: " << atlas->name << "\n";

        current = nullptr;
        return;
    }

    currentAnimPlaying = actionName;
    isDonePlaying = false;

    current = &it->second;
    current->frameTime = it->second.frameTime;
    current->loop = it->second.loop;
    frameIndex = 0;
    elapsed = 0.0f;
}

void UnitAnimator::Update(float elapsedTime)
{
    if (!current) return;
    elapsed += elapsedTime;
    float targetFrameTime{};
    if (generalFrameTime > 0.0f)
    {
        targetFrameTime = generalFrameTime;
    }
    else
    {
        targetFrameTime = current->frameTime;
    }
    if (elapsed >= targetFrameTime)
    {
        elapsed = 0.0f;
        frameIndex++;
        if (frameIndex >= (int)current->frameNames.size())
        {
            if (current->loop)
                frameIndex = 0;
            else
            {
                frameIndex = (int)current->frameNames.size() - 1;
                isDonePlaying = true;
            }
        }
    }

    // If it's a non-looping animation other than death
    if (isDonePlaying)
    {
        if (currentAnimPlaying != "death")
        {
            Play("breathing");
            isDonePlaying = false;
        }
    }
}

void UnitAnimator::DrawShade(std::initializer_list<std::pair<Color4f,Color4f>> colorPairs)
{
    if (!current || current->frameNames.empty()) return;
    const std::string& fname = current->frameNames[frameIndex];
    const FrameData& f = atlas->frames.at(fname);
    Rectf src{f.ToRectf()};
    texture->DrawShade(Rectf{0.0f,0.0f,float(f.w),float(f.h)},src,colorPairs);
}

bool UnitAnimator::IsDone() const
{ 
    return isDonePlaying;
}

float UnitAnimator::GetSpriteWidth()
{
    return float(atlas->frames.find(current->frameNames[0])->second.w);
}

std::string UnitAnimator::GetCurrentAnimation()
{
    return currentAnimPlaying;
}

void UnitAnimator::SetFrameTime(float frameTimeTarget)
{
    generalFrameTime = frameTimeTarget;
}