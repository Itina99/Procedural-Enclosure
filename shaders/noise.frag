#version 460 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in float height;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    return (ambient + diffuse + specular);
}

uniform int biomeId; // 0 = Mountains, 1 = Hills, 2 = Plains, 3 = Desert, 4 = Islands

// Funzione semplice di rumore basata su texCoords
float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 getBiomeColor(float h, int biomeId) {
    // Normalizza h (da [0, amplitude] a [0, 1])
    h = clamp(h, 0.0, 1.0); // Assumi già normalizzato, altrimenti dividi per amplitude

    float noise = rand(texCoords * 10.0); // puoi cambiare il "10.0" per aumentare/diminuire il dettaglio
    float distortedH = h + (noise - 0.5) * 0.05; // +/- 0.025 di disturbo
    if (biomeId == 0) { // Mountains
                        vec3 snowColor    = texture(texture_diffuse1, texCoords).rgb;
                        vec3 rockColor    = texture(texture_diffuse2, texCoords).rgb;
                        vec3 pebbleColor  = texture(texture_diffuse3, texCoords).rgb;
                        vec3 dirtColor    = texture(texture_diffuse4, texCoords).rgb;

                        float rockBlend   = smoothstep(0.6, 0.8, distortedH);
                        float pebbleBlend = smoothstep(0.4, 0.6, distortedH);
                        float dirtBlend   = smoothstep(0.2, 0.4, distortedH);

                        vec3 terrainColor = mix(dirtColor, pebbleColor, dirtBlend);
                        terrainColor = mix(terrainColor, rockColor, pebbleBlend);
                        terrainColor = mix(terrainColor, snowColor, rockBlend);

                        return terrainColor;
    }
    else if (biomeId == 1) { // Hills
                             vec3 lightGreen   = texture(texture_diffuse1, texCoords).rgb;
                             vec3 lushGreen    = texture(texture_diffuse2, texCoords).rgb;
                             vec3 darkGreen    = texture(texture_diffuse3, texCoords).rgb;
                             vec3 deepGreen    = texture(texture_diffuse4, texCoords).rgb;

                             float lushBlend   = smoothstep(0.4, 0.9, distortedH);
                             float darkBlend   = smoothstep(0.1, 0.4, distortedH);

                             vec3 terrainColor = mix(deepGreen, darkGreen, darkBlend);
                             terrainColor = mix(terrainColor, lushGreen, lushBlend);
                             terrainColor = mix(terrainColor, lightGreen, smoothstep(0.8, 0.9, distortedH));

                             return terrainColor;

    }
    else if (biomeId == 2) { // Plains (colori diretti)
                             vec3 lushGreen    = vec3(0.3, 0.7, 0.3);
                             vec3 lightGrass   = vec3(0.5, 0.8, 0.4);
                             vec3 dirtColor    = vec3(0.4, 0.3, 0.2);
                             vec3 waterColor   = vec3(0.0, 0.3, 0.5);

                             float lightBlend  = smoothstep(0.3, 0.6, h);
                             float dirtBlend   = smoothstep(0.1, 0.3, h);

                             vec3 terrainColor = mix(waterColor, dirtColor, dirtBlend);
                             terrainColor = mix(terrainColor, lightGrass, lightBlend);
                             terrainColor = mix(terrainColor, lushGreen, smoothstep(0.5, 0.6, h));

                             return terrainColor;
    }
    else if (biomeId == 3) { // Desert
                             vec3 rockyDesert  = texture(texture_diffuse1, texCoords).rgb;
                             vec3 sandColor    = texture(texture_diffuse2, texCoords).rgb;

                             float sandBlend   = smoothstep(0.5, 0.7, distortedH);

                             vec3 terrainColor = mix(sandColor, rockyDesert, sandBlend);

                             return terrainColor;
    }
    else if (biomeId == 4) { // Islands
                             vec3 jungleGreen  = texture(texture_diffuse1, texCoords).rgb;
                             vec3 sandColor    = texture(texture_diffuse2, texCoords).rgb;
                             vec3 oceanColor   = vec3(0.0, 0.4, 0.7);

                             // MODIFICA: allargata zona sabbia
                             float sandBlend   = smoothstep(0.05, 0.35, distortedH);

                             vec3 terrainColor = mix(oceanColor, sandColor, sandBlend);
                             terrainColor = mix(terrainColor, jungleGreen, smoothstep(0.3, 0.5, distortedH));

                             return terrainColor;
    }

    return vec3(1.0, 0.0, 1.0); // Magenta: fallback

}


void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    vec3 color = getBiomeColor(height, biomeId);

    FragColor = vec4(color, 1.0);

}
