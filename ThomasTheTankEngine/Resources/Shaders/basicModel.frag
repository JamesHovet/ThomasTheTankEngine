#version 330 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightDir;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fs_in;

uniform bool hasDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform bool hasNormalMap;
uniform sampler2D normalTexture;

uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;
uniform float time;

// old 
in vec3 Normal;

void main(){

    vec3 norm = texture(normalTexture, fs_in.texCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    // norm = Normal;


    vec3 baseColor = diffuseColor;
    // ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * baseColor;
    // diffuse 
    float diff = max(dot(fs_in.tangentLightDir, norm), 0.0);
    vec3 diffuse = diff * baseColor;
    // specular 
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec3 reflectDir = reflect(-fs_in.tangentLightDir, norm);
    vec3 halfwayDir = normalize(fs_in.tangentLightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * baseColor;

    vec3 lighting = ambient + diffuse + specular;

    vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
    FragColor = vec4(color * lighting, 1.0f);

//     vec3 sunLightDir = normalize(vec3(0.3f, 1.0, .5f));
    
//     vec3 baseColor = diffuseColor;

//     float ambientStrength = 0.2f;
//     vec3 ambient = ambientStrength * baseColor;

//     // totally incorrect way of doing it just to test:
// //    vec3 norm;
// //    if(hasNormalMap){
// //        vec3 normTex = texture(normalTexture, TexCoord).rgb;
// //        norm = normalize(normTex * 2.0f - 1.0f);
// //    } else {
// //        norm = normalize(Normal);
// //    }
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = sunLightDir;
//     float diff = max(dot(norm, lightDir), 0.0f);
//     vec3 diffuse = diff * baseColor;
    
//     float specularStrength = 0.5f;
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128);
//     vec3 specular = specularStrength * spec * baseColor;
    
//     vec3 result = ambient + diffuse + specular;
//     vec4 lighting = vec4(result, 1.0f);
//     if(hasDiffuseTexture){
//         FragColor = texture(diffuseTexture, TexCoord) * lighting;
//     } else {
//         FragColor = lighting;
//     }
    
//     vec3 Bitangent = Tangent.w * cross(norm, Tangent.xyz);
    
//    FragColor = vec4(vec3(1.0f, 1.0f, 1.0f) * norm, 1.0f);
//    FragColor = vec4(vec3(1.0f, 1.0f, 1.0f) * Tangent.rgb, 1.0f);
//    FragColor = vec4(vec3(1.0f, 1.0f, 1.0f) * Bitangent.rgb, 1.0f);

}
