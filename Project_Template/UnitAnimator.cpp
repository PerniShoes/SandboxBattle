#include "UnitAnimator.h"
#include "Texture.h"

UnitAnimator::UnitAnimator(Atlas* atlas,Texture* texture)
    : m_atlas(atlas),m_texture(texture)
{

}

void UnitAnimator::Play(const AnimationClip& data)
{
    auto it = m_atlas->animations.find(data.name);
    if (it == m_atlas->animations.end())
    {
        std::cerr << "Anim not found: " << data.name << "\n";
        m_current = nullptr;
        return;
    }
    m_current = &it->second;
    m_current->frameTime = data.frameTime;
    m_current->loop = data.loop;
    m_frameIndex = 0;
    m_elapsed = 0.0f;
}

void UnitAnimator::Update(float elapsedTime)
{
    if (!m_current) return;
    m_elapsed += elapsedTime;
    if (m_elapsed >= m_current->frameTime)
    {
        m_elapsed = 0.0f;
        m_frameIndex++;
        if (m_frameIndex >= (int)m_current->frameNames.size())
        {
            if (m_current->loop)
                m_frameIndex = 0;
            else
            {
                m_frameIndex = (int)m_current->frameNames.size() - 1;
            }
        }
    }
}

void UnitAnimator::DrawShade(std::initializer_list<std::pair<Color4f,Color4f>> colorPairs)
{
    if (!m_current || m_current->frameNames.empty()) return;
    const std::string& fname = m_current->frameNames[m_frameIndex];
    const FrameData& f = m_atlas->frames.at(fname);
    Rectf src{f.ToRectf()};
    m_texture->DrawShade(Rectf{0.0f,0.0f,float(f.w),float(f.h)},src,colorPairs);
}

bool UnitAnimator::IsPlaying() const 
{ 
    return m_current != nullptr; 
}
