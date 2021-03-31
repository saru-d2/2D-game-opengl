#include "powerup.h"

Powerup::Powerup(int x, int y)
{
    this->x = x;
    this->y = y;
    this->alive = true;
    if (rand() & 1)
    {
        //minus
        int numTriangles = 2;
        float verts[][3] = {
            {-0.3, 0.1, 0.0},
            {-0.3, -0.1, 0.0},
            {0.3, 0.1, 0.0},
            {0.3, -0.1, 0.0},
        };
        int triangles[][3] = {
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
                    vertex_buffer_data[9 * i + 3 * j + k] = verts[triangles[i][j]][k];
                }
            }
        }
        this->mod = -1;
        this->object = create3DObject(GL_TRIANGLES, numTriangles * 3, vertex_buffer_data, COLOR_RED, GL_FILL);
    }

    else
    {
        //plus
        int numTriangles = 4;
        float verts[][3] = {
            {-0.3, 0.1, 0.0},
            {-0.3, -0.1, 0.0},
            {0.3, 0.1, 0.0},
            {0.3, -0.1, 0.0},
            {-0.1, -0.3, 0.0},
            {-0.1, 0.3, 0.0},
            {0.1, -0.3, 0.0},
            {0.1, 0.3, 0.0},
        };
        int triangles[][3] = {
            {0, 1, 2},
            {1, 2, 3},
            {4, 5, 6},
            {5, 6, 7},

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
        this->mod = 1;
        this->object = create3DObject(GL_TRIANGLES, numTriangles * 3, vertex_buffer_data, COLOR_GREEN, GL_FILL);
    }
}

void Powerup::draw(glm::mat4 VP)
{
    if (!this->alive)
        return;
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

void Powerup::kill()
{
    this->alive = false;
}