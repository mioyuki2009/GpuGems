#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct WaveParam {
    vec2 direction;
    float frequency;
    float phase;
    float amplitude;
    float k;
}; 

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform float u_Time;
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform WaveParam wave1;
uniform WaveParam wave2;
uniform WaveParam wave3;


float sinWavePart1(vec2 pos)
{
    return sin(dot(wave1.direction,pos)*wave1.frequency +u_Time*wave1.phase) * 0.5+0.5;
}
float sinWavePart2(vec2 pos)
{
    return sin(dot(wave2.direction,pos)*wave2.frequency +u_Time*wave2.phase) * 0.5+0.5;
}
float sinWavePart3(vec2 pos)
{
    return sin(dot(wave3.direction,pos)*wave3.frequency +u_Time*wave3.phase) * 0.5+0.5;
}

float cosWavePart1(vec2 pos)
{
    return cos(dot(wave1.direction,pos)*wave1.frequency +u_Time*wave1.phase);
}
float cosWavePart2(vec2 pos)
{
    return cos(dot(wave2.direction,pos)*wave2.frequency +u_Time*wave2.phase);
}
float cosWavePart3(vec2 pos)
{
    return cos(dot(wave3.direction,pos)*wave3.frequency +u_Time*wave3.phase);
}

float waveNormalXPt1(float sinS, float cosS)
{
    return wave1.k * wave1.direction.x * wave1.frequency * wave1.amplitude * pow(sinS, wave1.k - 1) * cosS;
}

float waveNormalXPt2(float sinS, float cosS)
{
    return wave2.k * wave2.direction.x * wave2.frequency * wave2.amplitude * pow(sinS, wave2.k - 1) * cosS;
}

float waveNormalXPt3(float sinS, float cosS)
{
    return wave3.k * wave3.direction.x * wave3.frequency * wave3.amplitude * pow(sinS, wave3.k - 1) * cosS;
}

float waveNormalYPt1(float sinS, float cosS)
{
    return wave1.k * wave1.direction.y * wave1.frequency * wave1.amplitude * pow(sinS, wave1.k - 1) * cosS;
}

float waveNormalYPt2(float sinS, float cosS)
{
    return wave2.k * wave2.direction.y * wave2.frequency * wave2.amplitude * pow(sinS, wave2.k - 1) * cosS;
}

float waveNormalYPt3(float sinS, float cosS)
{
    return wave3.k * wave3.direction.y * wave3.frequency * wave3.amplitude * pow(sinS, wave3.k - 1) * cosS;
}


void main()
{
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vec2 fragXZ = FragPos.xz;

    float Wave1_temp=2*pow(sinWavePart1(fragXZ),wave1.k)*wave1.amplitude;
    float Wave2_temp=2*pow(sinWavePart2(fragXZ),wave2.k)*wave2.amplitude;
    float Wave3_temp=2*pow(sinWavePart3(fragXZ),wave3.k)*wave3.amplitude;

    float Wave1_cos=cosWavePart1(fragXZ);
    float Wave2_cos=cosWavePart1(fragXZ);
    float Wave3_cos=cosWavePart1(fragXZ);

    FragPos.y = Wave1_temp + Wave2_temp + Wave3_temp;

    float NormalX = waveNormalXPt1(Wave1_temp, Wave1_cos) + waveNormalXPt1(Wave2_temp, Wave2_cos) + waveNormalXPt1(Wave3_temp, Wave3_cos);
    float NormalY = waveNormalYPt1(Wave1_temp, Wave1_cos) + waveNormalYPt1(Wave2_temp, Wave2_cos) + waveNormalYPt1(Wave3_temp, Wave3_cos);

    vec3 NormalCalc = vec3(-NormalX, -NormalY, 1);

    Normal = mat3(transpose(inverse(u_Model))) * NormalCalc;  

    TexCoords = aTexCoords;    
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}

#type fragment
#version 450 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec4 waterColor;

void main()
{    
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
    
    vec3 result = ambient + specular;
    FragColor = vec4(diffuse * waterColor.xyz + result, waterColor.w);    
}