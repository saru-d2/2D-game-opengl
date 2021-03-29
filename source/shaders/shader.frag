#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragColor;
in vec4 vertPos;
// in vec4 agentPosition;
in vec3 normal;


uniform vec3 agentPos; 
// output data
uniform float lighted;
out vec3 color;



void main()
{
    float ambientStrength = 0.0;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
  	vec3 fragPos = vec3(vertPos.x, vertPos.y, vertPos.z);
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(agentPos - fragPos);
    float dist = (agentPos.x - fragPos.x) * (agentPos.x - fragPos.x) + (agentPos.y - fragPos.y) * (agentPos.y - fragPos.y);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    if (dist > 4) diffuse = vec3(0, 0, 0);
    vec3 result = (ambient + diffuse) * fragColor;
    // FragColor = vec4(result, 1.0);
    color = result;
    if (lighted > 0.0) color = fragColor;
}
