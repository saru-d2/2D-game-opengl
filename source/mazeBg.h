#include "main.h"

#ifndef MAZEBG_H
#define MAZEBG_H

class MazeBg
{
public:
    int r = 10;
    int c = 10;
    MazeBg() {}
    MazeBg(int r, int c);
    void draw(glm::mat4 VP);
    VAO *object;
    glm::vec3 position;
    float rotation;
};

#endif
