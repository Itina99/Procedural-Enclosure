#version 330 core
in vec2 texCoords;
out vec4 FragColor;

uniform float time;
uniform sampler2D dudvMap;

uniform float waveFrequency;
uniform float waveAmplitude;
uniform float waveSpeed;

void main() {
    // Calcola il centro (ipotizzando mesh centrata a 10,10 come prima)
    vec2 center = vec2(8.0, 8.0);
    vec2 radialCoords = texCoords * 8.0; // Portiamo tutto su scala 0-20 circa
    vec2 delta = radialCoords - center;

    // Distanza dal centro
    float dist = length(delta);

    // Direzione radiale normalizzata
    vec2 dir = normalize(delta);

    // Genera distorsione radiale sinusoidale
    float wave = sin(dist * waveFrequency + time * waveSpeed) * waveAmplitude;

    // Applica distorsione nella direzione radiale usando seno/coseno
    vec2 distortion = -dir * wave;

    // Mescola con una dudv map per dettaglio fine
    vec2 dudv = texture(dudvMap, texCoords + vec2(time * waveAmplitude)).rg * 0.02;

    vec2 finalCoords = texCoords + distortion + dudv;

    // Colore base + leggera variazione
    vec3 waterColor = vec3(0.0, 0.4, 0.6) + sin(dist * waveFrequency + time * waveSpeed) * 0.03;

    FragColor = vec4(waterColor, 0.5); // semitrasparente
}

