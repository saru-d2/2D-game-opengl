#include "mazeBg.h"
#include "main.h"

MazeBg::MazeBg(int r, int c)
{
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    this->r = r;
    this->c = c;
    float fR = (float) this->r;
    float fC = (float) this->c;
    int numTriangles = 2;
    GLfloat vertices[][3] = {
        {0.0f, 0.0f, 0.0f},       //0
        {0.0f, fR, 0.0f},    //0
        {fR, 0.0f, 0.0f},    //0
        {fR, fC, 0.0f}, //0
    };
    int triangleList[][3] = {
        {0, 1, 2},
        {1, 2, 3},
    };
    GLfloat vertex_buffer_data[9 * numTriangles];

    for (int i = 0; i < numTriangles; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // for each pt in each triangle
            for (int k = 0; k < 3; k++)
            {
                vertex_buffer_data[9 * i + 3 * j + k] = vertices[triangleList[i][j]][k];
            }
        }
    }

    this->object = create3DObject(GL_TRIANGLES, numTriangles * 3, vertex_buffer_data, COLOR_BACKGROUND, GL_FILL);
}

void MazeBg::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(-0.5, -0.5, 0)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    // std::cout << "hi" << std::endl;
}