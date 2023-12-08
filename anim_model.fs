#version 330 core
out vec4 FragColor;  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 brightLightColor = 2.0 * lightColor;  
    if (lightColor == vec3(1.0,  1.0, 1.0))
    {
        brightLightColor = lightColor;
    }

    vec3 diffuse = diff * brightLightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * brightLightColor;  
        
    vec3 result = (ambient + diffuse + specular) * vec3(texture(texture_diffuse1, TexCoords));
    FragColor = vec4(result, 1.0);

}