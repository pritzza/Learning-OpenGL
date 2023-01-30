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
	bool isPoint;

	vec3 position;
    vec3 direction;

    // light attenuation coefficents [constant, linear, quadratic]
	vec3 k;

    // cosine of cone's angle from center
    float innerCone;
	float outterCone;

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

vec3 getLightDirectionToFragment()
{
    return normalize(light.position - fragPosition);
}

// returns distance from fragment to light source
// can optimize by returning square of distance
float getDistance()
{
    vec3 displacement = (fragPosition - light.position);

    return length(displacement);
}

float getAttenuation()
{
    float d = getDistance();
    vec3 k = vec3(light.k);

    return 1 / (k.x + k.y * d + k.z * pow(d,2));
}

vec3 ambient(const vec2 ambientMapCoord)
{
    return texture(mappedMaterial.diffuse, ambientMapCoord).rgb * light.ambientColor;
}

vec3 diffuse(const vec2 diffuseMapCoord, const vec3 lightDirection)
{
    float diffuseStrength = max(dot(normal, lightDirection), 0);

    return diffuseStrength * texture(mappedMaterial.diffuse, diffuseMapCoord).rgb * light.diffuseColor;
}

vec3 specular(const vec2 specularMapCoord, const vec3 lightDirection)
{
    vec3 viewDirection = normalize(fragPosition - viewPosition);
    vec3 reflectionDirection = reflect(lightDirection, normal);

    float specularity = pow( max( dot(viewDirection, reflectionDirection), 0), mappedMaterial.shininess);

    return specularity * texture(mappedMaterial.specular, specularMapCoord).rgb * light.specularColor;
}

// returns 0-1 value of how much a fragment is in a spot light
float getLuminance(vec3 lightDirToFrag)
{
    float angle = dot(lightDirToFrag, light.direction);

    if (angle > light.innerCone)
        return 1;
    else if (angle > light.outterCone)
        return (angle - light.outterCone)/(light.innerCone - light.outterCone);

    return 0;
}

void main()
{
    vec3 lightDirToFrag = getLightDirectionToFragment();

    vec3 ambientColor = ambient(texCoord);
    vec3 diffuseColor = diffuse(texCoord, lightDirToFrag);
    vec3 specularColor = specular(texCoord, lightDirToFrag);

    float attenuation = 1;

    // only calculate attenuation if light is a point-light
    if (light.isPoint)
        attenuation = getAttenuation();

    float luminance = getLuminance(lightDirToFrag);

    vec3 finalColor = ambientColor + ((diffuseColor + specularColor) * attenuation) * luminance;

    FragColor = vec4(finalColor, 1.0);
}