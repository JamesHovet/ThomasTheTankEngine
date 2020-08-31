#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform bool hasDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform bool hasNormalMap;
uniform sampler2D normalTexture;

uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;

void main(){
    
    vec3 sunLightDir = normalize(vec3(0.3f, 1.0, .5f));
    
    vec3 baseColor = diffuseColor;

    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * baseColor;

    // totally incorrect way of doing it just to test:
//    vec3 norm;
//    if(hasNormalMap){
//        vec3 normTex = texture(normalTexture, TexCoord).rgb;
//        norm = normalize(normTex * 2.0f - 1.0f);
//    } else {
//        norm = normalize(Normal);
//    }
    vec3 norm = normalize(Normal);
    vec3 lightDir = sunLightDir;
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * baseColor;
    
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128);
    vec3 specular = specularStrength * spec * baseColor;
    
    vec3 result = ambient + diffuse + specular;
    vec4 lighting = vec4(result, 1.0f);
    if(hasDiffuseTexture){
        FragColor = texture(diffuseTexture, TexCoord) * lighting;
    } else {
        FragColor = lighting;
    }

}
