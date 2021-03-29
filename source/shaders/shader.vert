#version 330 core

// input data : sent from main program
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
uniform mat4 MVP;

// output data : used by fragment shader
out vec3 fragColor;
out vec4 vertPos;
// uniform vec3 agentPos; 
out vec4 agentPosition;
out vec3 normal;

void main ()
{
    vec4 v = vec4(vertexPosition, 1); // Transform an homogeneous 4D vector

    normal = vec3(0, 0, -1.0);
    vertPos = vec4(vertexPosition, 1.0);

    fragColor = vertexColor;

    gl_Position  = MVP * v;
}
