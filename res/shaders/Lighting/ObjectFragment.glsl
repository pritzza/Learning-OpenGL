#version 330 core

// for diffuse lighting
in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

// for specular lighting
uniform vec3 lightSourcePosition;
uniform vec3 viewPosition;

// for basic lighting
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient lighting
    const float ambientStrength = 0.3;
    vec3 ambientColor = ambientStrength * lightColor;

    // diffuse lighting
    vec3 lightDirection = normalize(lightSourcePosition - fragPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0);
    vec3 diffuseColor = diffuseStrength * lightColor;

    // specular lighting
    vec3 viewDirection = normalize(fragPosition - viewPosition);
    vec3 reflectionDirection = reflect(lightDirection, normal);

    const float shininess = 32;
    float specular = pow( max( dot(viewDirection, reflectionDirection), 0), shininess);
    
    const float specularStrength = 0.5;
    vec3 specularColor = specularStrength * specular * lightColor;


    vec3 finalColor = (ambientColor + diffuseColor + specular) * objectColor;

    FragColor = vec4(finalColor, 1.0);
} 