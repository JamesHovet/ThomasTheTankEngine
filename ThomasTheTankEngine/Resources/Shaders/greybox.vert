#version 330 core
layout (location = 0) in vec3 vpos;    // vertex position
layout (location = 1) in vec4 model0;
layout (location = 2) in vec4 model1;
layout (location = 3) in vec4 model2;
layout (location = 4) in vec4 model3;
layout (location = 5) in vec4 color;

out vec4 vert_color;

uniform mat4 view;
uniform mat4 projection;


void main()
{
    mat4 model = mat4(model0, model1, model2, model3);
    vert_color = color;
    gl_Position = projection * view * model * vec4(vpos, 1.0f);
}

