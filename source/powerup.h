#include "main.h"

#ifndef POWERUP_H
#define POWERUP_H

class Powerup
{
public:
    int x;
    int y;
    Powerup() {}
    Powerup(int r, int c);
    void draw(glm::mat4 VP);
    VAO *object;
    glm::vec3 position;
    float rotation;
    int mod;
    bool alive;
    void kill();

};

#endif
