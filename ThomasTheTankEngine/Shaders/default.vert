#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;

uniform vec2 shift;
uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = projection * view * model * pos;
    
    ourColor = aPos.xyz; // set ourColor to the input color we got from the vertex data
    TexCoord = aTexCoord;
}
