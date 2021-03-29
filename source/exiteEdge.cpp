#include "exitEdge.h"

ExitEdge::ExitEdge(std::pair<int, int> outXY, int r, int c)
{
    this->x = outXY.first;
    this->y = outXY.second;
    float vertex_buffer_data[6];
    float verts[][3] = {
        {-0.5, -0.5, 0.01},
        {-0.5, 0.5, 0.01},
        {0.5, -0.5, 0.01},
        {0.5, 0.5, 0.01},
    };
    std::pair<int, int> pts;
    if (this->x == 0)
    {
        pts = {0, 1};
    }
    else if (this->x == r - 1)
    {
        pts = {3, 2};
    }

    if (this->y == 0)
    {
        pts = {0, 2};
    }
    else if (this->y == c - 1)
    {
        pts = {1, 3};
    }

    for (int i =0; i<3; i++) vertex_buffer_data[i] = verts[pts.first][i];
    for (int i =0; i<3; i++) vertex_buffer_data[i + 3] = verts[pts.second][i];


    this->object = create3DObject(GL_LINES, 2, vertex_buffer_data, COLOR_BACKGROUND, GL_FILL);
}

void ExitEdge::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(this->x, this->y, 0)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    // std::cout << "hi" << std::endl;
}