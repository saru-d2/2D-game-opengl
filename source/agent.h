#include "main.h"

#ifndef AGENT_H
#define AGENT_H

class Agent
{
public:
    Agent() {}
    Agent(float x, float y, bool);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    float x, y;
    void seek(int, int, std::vector<std::vector<int>>, int, int);
    void move(int , int, std::vector<std::vector<int>>, int, int);
    bool alive;
    int health;
    void kill();
private:
    VAO *object;
};

#endif 
