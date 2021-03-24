#include "main.h"

#ifndef AGENT_H
#define AGENT_H


class Agent {
public:
    Agent() {}
    Agent(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
