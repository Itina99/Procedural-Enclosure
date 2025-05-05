#version 460

in vec2 tCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 color;


struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
};

uniform vec3 vPos;
uniform sampler2D diffuse;
uniform DirLight light;


void main() {
    vec4 t_color = texture(diffuse, tCoords);
    vec3 ambient = light.ambient * texture(diffuse, tCoords).rgb;
    vec3 norm = normalize(normal);
    vec3 lightdir = normalize(-light.direction);
    float diff = max(dot(norm, lightdir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(diffuse, tCoords).rgb;

    vec3 result = ambient + diffuse;
    if(t_color.a < 0.9){
        discard;
    }
    color = vec4(result, 1.0);
}