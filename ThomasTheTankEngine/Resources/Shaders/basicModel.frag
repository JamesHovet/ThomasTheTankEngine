#version 330 core
layout(location = 0) out vec4 FragColor;

in vec4 colorOut;

void main(){
//    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    FragColor = colorOut;
}
