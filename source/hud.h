#include <ft2build.h>
#include "main.h"
#include FT_FREETYPE_H

class Agent;

class HUD
{
public:
    HUD() {}
    HUD(int, int, int, int);
    void RenderText(GLuint shaderID, std::string text, float x, float y, float scale, glm::vec3 color);
    int x, y, height, width;
};