#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main(){
    vec4 pos = vec4(aPos.x / 2.0f, aPos.y / 2.0f, aPos.z / 2.0f, 1.0f);
    gl_Position = projection * view * model * pos;
//    gl_Position =  model * pos;
    
//    gl_Position = mat4(1.0f) * pos;
    
    TexCoord = aTexCoord;
}
