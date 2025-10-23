#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

out vec2 TexCoords;


void main()
{
    vec3 pos = aPos; // Kopiujemy pozycję

    // --- LOGIKA FALOWANIA ---
    // Używamy pozycji X i Z bloku oraz czasu, aby stworzyć falę
    // Amplituda fali (jak wysoka) to 0.08
    // Prędkość fali to time * 2.0
    pos.y += sin(pos.x * 0.5 + pos.z * 0.5 + time * 2.0) * 0.08;
    gl_Position = projection*view*model*vec4(pos,1.0f);
    TexCoords = aTexCoords;
}
