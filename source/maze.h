#include "main.h"

#ifndef MAZE_H
#define MAZE_H

class Maze
{
public:
    std::vector<std::vector<int>> adj;
    int r = 10;
    int c = 10;
    Maze() {}
    Maze(int r, int c);
    std::pair<int, int> outCoords;
    void draw(glm::mat4 VP);
    VAO *object;
    glm::vec3 position;
    float rotation;
};

#endif
