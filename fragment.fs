#version 330 core
struct Material {
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  
  
uniform Material material;


out vec4 FragColor;

in vec2 texCoords;
in vec3 Normal;  
in vec3 FragPos;  
in mat4 mView;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

void main()
{
    int i=0;
    // ambient
    vec3 ambient = vec3(texture(material.texture_diffuse1,texCoords)) * light.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff*vec3(texture(material.texture_diffuse1,texCoords))) * light.diffuse;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = vec3(texture(material.texture_specular1,texCoords))* spec * light.specular;  
        
    vec3 result = ambient + diffuse + specular ;
    FragColor = vec4(result, 1.0);
} 