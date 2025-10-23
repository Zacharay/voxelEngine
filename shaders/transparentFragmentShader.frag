#version 430 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D textureAtlas;

void main()
{
    vec4 texColor = texture(textureAtlas, TexCoords);
    if(texColor.a < 0.1)
    discard;

    FragColor = vec4(texColor.rgb, texColor.a * 0.6);

}