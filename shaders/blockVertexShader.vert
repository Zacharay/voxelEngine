#version 430 core

layout(location = 0)in uint packedVertexData;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out float aAo;

// Vert 0 (TL): (min, max) -> (0, 1)
// Vert 1 (TR): (max, max) -> (1, 1)
// Vert 2 (BR): (max, min) -> (1, 0)
// Vert 3 (BR): (max, min) -> (1, 0)
// Vert 4 (BL): (min, min) -> (0, 0)
// Vert 5 (TL): (min, max) -> (0, 1)
const vec2 uvOffsets[6] = vec2[6](
vec2(0.0, 1.0), // Vert 0
vec2(1.0, 1.0), // Vert 1
vec2(1.0, 0.0), // Vert 2
vec2(1.0, 0.0), // Vert 3
vec2(0.0, 0.0), // Vert 4
vec2(0.0, 1.0)  // Vert 5
);

void main()
{

    float posY = float(bitfieldExtract(packedVertexData, 0, 9));
    float posX = float(bitfieldExtract(packedVertexData, 9, 5));
    float posZ = float(bitfieldExtract(packedVertexData, 14, 5));


    float texX = float(bitfieldExtract(packedVertexData, 19, 5));
    float texY = float(bitfieldExtract(packedVertexData, 24, 5));


    uint packedAo = bitfieldExtract(packedVertexData, 29, 3);
    float normalizedAo = float(packedAo) / 7.0;


    aAo = mix(0.3, 1.0, normalizedAo);


    int vertexIndex = gl_VertexID % 6;
    vec2 offset = uvOffsets[vertexIndex];

    TexCoords = (vec2(texX, texY) + offset) / 32.0;

     gl_Position = projection * view * model * vec4(posX, posY, posZ, 1.0f);
}
