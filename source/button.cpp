#include "button.h"

Button::Button(int x, int y, int power)
{
    this->power = power;
    this->x = x;
    this->y = y;
    this->alive = true;

    int numTriangles = 100;
    float TwoPi = 2 * M_PI;
    float theta1 = 0;
    float theta2 = 0;

    GLfloat vertex_buffer_data[9 * numTriangles];
    float radius = 0.4;

    for (int i = 0; i < numTriangles; i++)
    {
        theta1 = theta2;
        theta2 += TwoPi / (float)numTriangles;

        float pts[][3]{
            {0, 0, 0},
            {cos(theta1) * radius, sin(theta1) * radius, 0},
            {cos(theta2) * radius, sin(theta2) * radius, 0},
        };
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
                vertex_buffer_data[9 * i + 3 * j + k] = pts[j][k];
        }
    }

    color_t color;
    if (this->power == 0) color = COLOR_RED;
    else color = COLOR_GREEN;

    this->object = create3DObject(GL_TRIANGLES, numTriangles * 3, vertex_buffer_data, color, GL_FILL);
}


void Button::draw(glm::mat4 VP)
{
    if (!this->alive)return;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(this->x, this->y, 0)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Button::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Button::tick()
{
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
