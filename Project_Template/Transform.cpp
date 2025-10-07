#include "Transform.h"
#include <SDL_opengl.h>

Transform::Transform(Point2f position,Point2f scale, float angle)
    :position{position}
    ,scale{scale}
    ,angle{angle}
    ,hitboxWidth{}
{

}

void Transform::Push()const
{
    glPushMatrix();
}
void Transform::Apply() const
{
    // Position.x/y is based on middle of the sprite. So I translate it by half and a bit. So units move "onto" cursor, not "into" it
    glTranslatef(position.x,position.y+(hitboxWidth/2.0f * abs(scale.y)-(25.0f*abs(scale.y))),0);
    glScalef(scale.x, scale.y, 0);
    glRotatef(angle,0,0,1);

    glTranslatef(-hitboxWidth/2.0f,-hitboxWidth/2.0f,0);

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
    float hitboxOffsetX{(hitboxWidth / 2.0f) * abs(scale.x)};
    float hitboxOffsetY{(hitboxWidth / 2.0f) * abs(scale.y)};
    // -(25.0f) explanation above in Transform::Apply()
    return Rectf{position.x - hitboxOffsetX,position.y - (25.0f * abs(scale.y)),hitboxWidth * abs(scale.x),hitboxWidth * abs(scale.y)};
}

void Transform::Scale(float x,float y)
{
    scale.x *= x;
    scale.y *= y;
}

void Transform::SetExactScale(float x,float y) const
{
    scale.x = x;
    scale.y = y;
}