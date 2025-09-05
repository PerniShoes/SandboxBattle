#include "Transform.h"
#include <SDL_opengl.h>

Transform::Transform(Point2f position,Point2f scale, float angle)
    :position{position}
    ,scale{scale}
    ,angle{angle}
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

}

void Transform::Pop()const
{
    glPopMatrix();
}


