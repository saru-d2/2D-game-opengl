#include "main.h"

#ifndef EXITEDGE_H
#define EXITEDGE_H

class ExitEdge
{
public:
    int x;
    int y;
    ExitEdge() {}
    ExitEdge(std::pair<int, int>, int, int);
    void draw(glm::mat4 VP);
    VAO *object;
    glm::vec3 position;
    float rotation;
};

#endif