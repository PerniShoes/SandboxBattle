#pragma once
#include "utils.h"

struct Transform
{
    Transform(Point2f position = {},Point2f scale = Point2f{1.0f,1.0f},float angle = 0.0f);

    void Push()const;
    void Apply()const;
    void Pop()const;

    void FlipX();
    void FlipY();
    Rectf GetHitbox();
    
    Point2f position;
    Point2f scale;
    float angle;
    float hitboxWidth;

};

