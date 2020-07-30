#version 330 core
layout (location = 0) in vec3 vpos;    // vertex position
layout (location = 1) in vec3 ipos;   // position from instance stuff
layout (location = 2) in vec3 scale;
layout (location = 3) in vec4 color;
layout (location = 4) in vec4 rotation;
out vec4 vert_color;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


vec3 qtransform( vec4 q, vec3 v ){
   return v + 2.0*cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}

void main()
{
//    vert_color = color;
    vert_color = color;
//    gl_Position = projection * view * vec4(ipos + (vpos), 1.0f);
    vec3 scaled = vec3(vpos.x * scale.x, vpos.y * scale.y, vpos.z * scale.z);
    vec3 rotated = qtransform(rotation, scaled);
    gl_Position = projection * view * vec4(ipos + rotated, 1.0f);
}

