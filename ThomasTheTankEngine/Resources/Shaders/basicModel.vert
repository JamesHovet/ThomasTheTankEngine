#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out vec2 TexCoord;
out vec4 colorOut;

void main(){
    
    vec3 lightPos = vec3(1.0f, 2.0f, 2.0f);
    vec3 lightColor = vec3(1.0f, 0.5f, 0.2f);
    
    vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vec3 worldPos = (model * pos).xyz;
    vec3 norm = aNormal; // todo: the special "Normal Matrix" https://learnopengl.com/Lighting/Basic-Lighting
    
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    
    
    // diffuse
    vec3 lightDir = normalize(lightPos - worldPos);
    float diffuseStrength = max(dot(lightDir, normalize(norm)), 0.0f);
    vec3 diffuse = diffuseStrength * lightColor;
    
    // specular
    float specularStrengthScale = 0.5f;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * specularStrengthScale * lightColor;
    
    gl_Position = projection * view * model * pos;
    vec3 color = ambient + diffuse + specular;
    
    colorOut = vec4(color, 1.0f);
    TexCoord = aTexCoord;
}
