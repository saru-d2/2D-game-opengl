#include "main.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:
    Button() {};
    Button(int x, int y, int power);
    glm::vec3 position;
    int power;
    int x, y;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    VAO *object;
    bool alive;
};

#endif