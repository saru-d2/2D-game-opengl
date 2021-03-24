#include "agent.h"
#include "main.h"

Agent::Agent(float x, float y, color_t color)
{

    int numTriangles = 6;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    GLfloat vertices[][3] = {
        {0.0f, 0.5f, 0.0f},   //0
        {-0.3f, 0.4f, 0.0f},  //1
        {0.3f, 0.4f, 0.0f},   //2
        {-0.3f, 0.3f, 0.0f},  //3
        {-0.4f, 0.3f, 0.0f},  //4
        {-0.3f, -0.3f, 0.0f}, //5
        {0.3f, -0.3f, 0.0f},  //6
        {0.0f, -0.3f, 0.0f},  //7
        {0.3f, -0.5f, 0.0f},  //8
        {-0.3f, -0.5f, 0.0f}, //9
    };

    int triangleList[][3] = {
        {0, 1, 2},
        {1, 2, 5},
        {2, 5, 6},
        {3, 4, 5},
        {5, 7, 8},
        {6, 7, 9}};

    GLfloat vertex_buffer_data[9 * numTriangles];

    for (int i = 0; i < numTriangles; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // for each pt in each triangle
            for (int k = 0; k < 3; k++)
            {
                vertex_buffer_data[9 * i + 3 * j + k] = vertGuide[triangleList[i][j]][k];
            }
        }
    }

    this->object = create3DObject(GL_LINES, numTriangles * 3, vertex_buffer_data, color, GL_FILL);
}

void Agent::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Agent::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Agent::tick()
{
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
