#version 330 core

out vec4 FragColor;

in VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} fs_in;

uniform sampler2D sampler;

uniform vec3 lightPos[4];
uniform vec3 viewPos;

void main()
{
    const float ambientFactor = 0.05;
    const float shininess = 16.0;
    const vec3 lightColor = vec3(0.3);

    vec3 diffuseColor = vec3(0);
    vec3 specularColor = vec3(0);

    vec3 baseColor = texture(sampler, fs_in.texCoords).rgb;

    // Ambient
    vec3 ambientColor = baseColor * ambientFactor;

    for(int i=0; i < 4; i++)
    {
        // Diffuse
        vec3 lightDir = normalize(lightPos[i] - fs_in.fragPos);
        vec3 normal = normalize(fs_in.normal);
        float diffuseFactor = max(dot(lightDir, normal), 0.0);
        diffuseColor += baseColor * diffuseFactor;
    
        // Specular
        vec3 viewDir = normalize(viewPos - fs_in.fragPos);
        vec3 halfwaydir = normalize(viewDir + lightDir);
        float specularFactor = pow(max(dot(halfwaydir, normal), 0.0), shininess);
        specularColor += specularFactor * lightColor;
    }

    // Final Color;
    FragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}