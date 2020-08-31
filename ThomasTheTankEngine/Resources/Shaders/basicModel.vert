#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;

out vec2 TexCoord;

out vec3 Normal;
out vec3 FragPos;

void main(){
    
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = normalMatrix * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0f);
}
