#version 330 core

// taken from src/gfx/Lighting.h
// MAKE SURE THE STRUCTURES ARE IDENTICAL
struct MappedMaterial
{
	sampler2D diffuse;  // diffuse and ambient light map
	sampler2D specular;	// specular light map
	float shininess;    // scattering/radius of specular highlight
};

struct Material
{
	vec3 ambientColor;  // color under ambient lighting
	vec3 diffuseColor;  // color under diffuse lighting (surface color)
	vec3 specularColor;	// color of specular highlight

	float shininess;    // scattering/radius of specular highlight
};

struct Light
{
	vec3 position;		// light source position

	vec3 ambientColor;	// intensity of ambient light in each RGB component
	vec3 diffuseColor;	// color of light source
	vec3 specularColor;	// generall left at vec3(1.0) for max shining
};

// for lighting
in vec3 normal;
in vec3 fragPosition;
in vec2 texCoord;

out vec4 FragColor;

// for specular lighting
uniform vec3 viewPosition;

//uniform Material material;
uniform MappedMaterial mappedMaterial;
uniform Light light;

void main()
{
    // ambient lighting
    vec3 ambientColor = texture(mappedMaterial.diffuse, texCoord).rgb * light.ambientColor;

    // diffuse lighting
    vec3 lightDirection = normalize(light.position - fragPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0);
    //vec3 diffuseColor = diffuseStrength * vec3(texture(mappedMaterial.diffuse, texCoord)) * light.diffuseColor;
    vec3 diffuseColor = texture(mappedMaterial.diffuse, texCoord).rgb * light.diffuseColor;
    //diffuseColor = vec3(0);


    // specular lighting
    vec3 viewDirection = normalize(fragPosition - viewPosition);
    vec3 reflectionDirection = reflect(lightDirection, normal);

    float specular = pow( max( dot(viewDirection, reflectionDirection), 0), mappedMaterial.shininess);
    vec3 specularColor = specular * texture(mappedMaterial.specular, texCoord).rgb * light.specularColor;
    //specularColor = vec3(0);

    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    FragColor = vec4(finalColor, 1.0);
} 