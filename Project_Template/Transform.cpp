#include "Transform.h"
#include <SDL_opengl.h>

Transform::Transform(Point2f position,Point2f scale, float angle)
    :position{position}
    ,scale{scale}
    ,angle{angle}
    ,hitboxWidth{}
    ,hitboxHeight{}
    ,offsetY{}
    ,offsetX{}
{

}

void Transform::Push()const
{
    glPushMatrix();
}
void Transform::Apply() const
{
    // Offset used to center things instead of drawing from bottom
    glTranslatef(position.x + (offsetX * abs(scale.x)) ,position.y + (offsetY *abs(scale.y)),0); // Same here
    glScalef(scale.x, scale.y, 0);
    glRotatef(angle,0,0,1);

}
void Transform::Translate() const
{
    glTranslatef(position.x + (offsetX * abs(scale.x)),position.y + (offsetY * abs(scale.y)),0); // Same here
}
void Transform::MoveFromOrigin() const
{
    glTranslatef((hitboxWidth / 2.0f) * abs(scale.x), (hitboxHeight / 2.0f) * abs(scale.y),0);
}
void Transform::MoveToOrigin() const
{
    glTranslatef(-hitboxWidth / 2.0f,-hitboxHeight / 2.0f,0);
}
void Transform::Pop()const
{
    glPopMatrix();
}

void Transform::FlipX()
{
    scale.x = -scale.x;
}
void Transform::FlipY()
{
    scale.y = -scale.y;
}
Rectf Transform::GetHitbox() const
{
    float hitboxOffsetX{(hitboxWidth / 2.0f) * abs(scale.x)};  // To erase
    float hitboxOffsetY{(hitboxHeight / 2.0f) * abs(scale.y)}; // To erase
    // -(25.0f) explanation above in Transform::Apply()
    return Rectf{position.x + (offsetX * abs(scale.x))
        ,position.y + (offsetY * abs(scale.y)),hitboxWidth * abs(scale.x),hitboxHeight * abs(scale.y)};
}

void Transform::Scale(float x,float y)
{
    scale.x *= x;
    scale.y *= y;
}

void Transform::SetExactScale(float x,float y) 
{
    scale.x = x;
    scale.y = y;
}