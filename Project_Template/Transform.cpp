#include "Transform.h"
#include <SDL_opengl.h>

Transform::Transform(Point2f position,Point2f scale)
    :position{position}
    ,scale{scale}
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

}

void Transform::Pop()const
{
    glPopMatrix();
}


