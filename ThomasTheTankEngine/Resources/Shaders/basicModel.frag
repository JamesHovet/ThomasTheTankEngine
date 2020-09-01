#version 330 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightDir;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 normalFallback;
} fs_in;

uniform bool hasDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform bool hasNormalMap;
uniform sampler2D normalTexture;

uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularExp;

uniform vec3 lightColor;
uniform float time;

void main(){

    vec3 norm;
    if(hasNormalMap){
        norm = texture(normalTexture, fs_in.texCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
    } else {
        norm = fs_in.normalFallback;
    }
    
    // ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor * ambientColor;
    // diffuse 
    float diff = max(dot(fs_in.tangentLightDir, norm), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor;
    // specular 
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec3 reflectDir = reflect(-fs_in.tangentLightDir, norm);
    vec3 halfwayDir = normalize(fs_in.tangentLightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), specularExp);
    vec3 specular = spec * specularColor * lightColor;

    vec3 lighting = ambient + diffuse + specular;
    vec3 color = diffuseColor;
    
    if(hasDiffuseTexture){
        color = color * texture(diffuseTexture, fs_in.texCoords).rgb;
    }
    
    FragColor = vec4(color * lighting, 1.0f);

}
