#include "UnitAnimator.h"
#include "Texture.h"

UnitAnimator::UnitAnimator(Atlas* atlas,Texture* texture)
    : atlas(atlas),texture(texture)
{

}

void UnitAnimator::Play(const std::string actionName)
{
    auto it = atlas->animations.find(actionName);
    if (it == atlas->animations.end())
    {
        std::cerr << "Anim not found: " << actionName << "\n";
        current = nullptr;
        return;
    }

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
    if (elapsed >= current->frameTime)
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
