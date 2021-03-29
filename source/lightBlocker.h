#include "main.h"

#ifndef LIGHTBLOCKER_H
#define LIGHTBLOCKER_H

class LightBlocker
{
public:
    int x;
    int y;
    LightBlocker() {}
    LightBlocker(int r, int c);
    void draw(glm::mat4 VP);
    VAO *object;
    glm::vec3 position;
    float rotation;
};

#endif
