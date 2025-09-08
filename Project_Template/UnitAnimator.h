//#pragma once
//
//#include "Texture.h"
//#include "SpriteAtlasManager.h"
//#include "utils.h"
//
//class UnitAnimator
//{
//public:
//    UnitAnimator(Atlas* atlas,Texture* texture)
//        : m_atlas(atlas),m_texture(texture)
//    {
//    }
//
//    void Play(float frameTime = 0.1f,bool loop = true)
//    {
//        auto it = m_atlas->animations.find(data.animName);
//        if (it == m_atlas->animations.end())
//        {
//            std::cerr << "Anim not found: " << data.animName << "\n";
//            m_current = nullptr;
//            return;
//        }
//        m_current = &it->second;
//        m_current->frameTime = data.frameTime;
//        m_current->loop = data.loop;
//        m_frameIndex = 0;
//        m_elapsed = 0.0f;
//    }
//
//    void Update(float dt)
//    {
//        if (!m_current) return;
//        m_elapsed += dt;
//        if (m_elapsed >= m_current->frameTime)
//        {
//            m_elapsed = 0.0f;
//            m_frameIndex++;
//            if (m_frameIndex >= (int)m_current->frameNames.size())
//            {
//                if (m_current->loop)
//                    m_frameIndex = 0;
//                else
//                {
//                    m_frameIndex = (int)m_current->frameNames.size() - 1;
//                }
//            }
//        }
//    }
//
//    bool IsPlaying() const { return m_current != nullptr; }
//
//private:
//    Atlas* m_atlas;
//    Texture* m_texture;
//
//    Rectf* m_current = nullptr;
//    int m_frameIndex = 0;
//    float m_elapsed = 0.0f;
//};
