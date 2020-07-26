#version 330 core
layout (location = 0) in vec3 vpos;    // vertex position
layout (location = 1) in vec3 ipos;   // position from instance stuff
layout (location = 2) in vec3 scale;
layout (location = 3) in vec4 color;
out vec4 vert_color;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//    vert_color = color;
    vert_color = color;
//    gl_Position = projection * view * vec4(ipos + (vpos), 1.0f);
    gl_Position = projection * view * vec4(ipos + vec3(vpos.x * scale.x,
                                                         vpos.y * scale.y,
                                                        vpos.z * scale.z), 1.0f);
}
