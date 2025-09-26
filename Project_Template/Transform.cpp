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

    glTranslatef(position.x,position.y,0);
    glScalef(scale.x, scale.y, 0);
    glRotatef(angle,0,0,1);

    glTranslatef(-hitboxWidth /2.0f,-hitboxWidth/2.0f,0);

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
Rectf Transform::GetHitbox()
{
    float hitboxOffsetX{(hitboxWidth / 2.0f) * abs(scale.x)};
    float hitboxOffsetY{(hitboxWidth / 2.0f) * abs(scale.y)};
    return Rectf{position.x - hitboxOffsetX,position.y- hitboxOffsetY,hitboxWidth * abs(scale.x),hitboxWidth * abs(scale.y)};
}

void Transform::Scale(float x,float y)
{
    scale.x *= x;
    scale.y *= y;
}
