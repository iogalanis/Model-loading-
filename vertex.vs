#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout(location =2 )  in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
out mat4 mView;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3( model *  vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    texCoords=texCoord;
    gl_Position = projection *view* vec4(FragPos, 1.0);
    mView=view;
}