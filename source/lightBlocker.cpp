#include "lightBlocker.h"

LightBlocker::LightBlocker(int x, int y){
    this -> x = x;
    this -> y = y;

    int numTriangles = 2;

    GLfloat verts[][3] = {
        {-0.5, -0.5, 0.0},
        {-0.5, 0.5, 0.0},
        {0.5, -0.5, 0.0},
        {0.5, 0.5, 0.0},
    };

    int triangles[][3] = {
        {0, 1, 3},
        {0, 3, 2}
    };

    GLfloat vertex_buffer_data[9 * numTriangles];

    for (int i = 0; i < numTriangles; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // for each pt in each triangle
            for (int k = 0; k < 3; k++)
            {
                vertex_buffer_data[9 * i + 3 * j + k] = verts[triangles[i][j]][k];
            }
        }
    }

    this->object = create3DObject(GL_TRIANGLES, numTriangles * 3, vertex_buffer_data, COLOR_NIGHT, GL_FILL);
}

void LightBlocker::draw(glm::mat4 VP)
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