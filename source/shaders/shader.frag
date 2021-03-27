#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragColor;
in vec4 vertPos;
in vec4 agentPosition;
// output data
out vec3 color;

void main()
{
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices of the triangle
    float dist = sqrt(dot(agentPosition, vertPos));
    // dist = max(1, dist);
    color = fragColor / dist;
    // color = fragColor;
}
