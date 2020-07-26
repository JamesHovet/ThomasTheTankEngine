#version 330 core
layout (location = 0) in vec3 vpos;   // vertex position
layout (location = 1) in vec3 ipos;   // position from instance stuff

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//    vec4 pos = vec4(vpos, 1.0);
    
    gl_Position = projection * view * vec4(vpos + ipos, 1.0f);
}
