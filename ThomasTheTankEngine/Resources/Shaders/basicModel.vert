#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;
uniform float time;

out VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightDir;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

// out vec3 FragPos;
// out vec2 TexCoord;
out vec3 Normal;
// out vec4 Tangent;

void main(){

    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.texCoords = aTexCoord;
    
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent.xyz);
    T = normalize(T - dot(T, N) * N); // orthogonalize T and N
    vec3 B = cross(N, T) * aTangent.w;
    mat3 TBN = transpose(mat3(T, B, N));

    vec3 sunLightDir = normalize(vec3(0.3f * sin(time), 1.0, .5f * cos(time)));

    vs_out.tangentLightDir = TBN * sunLightDir;
    vs_out.tangentViewPos = TBN * viewPos;
    vs_out.tangentFragPos = TBN * vs_out.fragPos;

    gl_Position = projection * view * vec4(vs_out.fragPos, 1.0f);

    // old
    // FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(normalMatrix * aNormal);
    // TexCoord = aTexCoord;
    // Tangent = vec4(normalize(normalMatrix * ((aTangent.xyz * 2) - 1)), 1.0f);
}
