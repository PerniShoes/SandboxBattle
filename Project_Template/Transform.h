#pragma once
#include "utils.h"

// Should be refactored in the future. hitbox and (optionaly) offsets need to be set for it to work,
// Should be in ctor or have func for those
struct Transform
{
    Transform(Point2f position = {},Point2f scale = Point2f{1.0f,1.0f},float angle = 0.0f);

    void Push()const;
    void Apply()const;
    void Pop()const;

    void FlipX();
    void FlipY();
    Rectf GetHitbox() const;
    void Scale(float x,float y);
    void SetExactScale(float x,float y) const;
    
    Point2f position;
    // FIX mutable just for testing
    mutable Point2f scale;
    float angle;
    float hitboxWidth;
    float hitboxHeight;
    float offsetY;
    float offsetX;

};

