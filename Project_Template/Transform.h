#pragma once
#include "utils.h"

struct Transform
{
    Transform(Point2f position = {},Point2f scale = Point2f{1.0f,1.0f});

    void Push()const;
    void Apply()const;
    void Pop()const;

    Point2f position;
    Point2f scale;

};

