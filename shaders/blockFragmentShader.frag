#version 430 core

out vec4 FragColor;
in vec2 TexCoords;
in float aAo;

uniform sampler2D textureAtlas;

void main(){
    vec4 tex = texture(textureAtlas, TexCoords);
    FragColor = vec4(tex.rgb * aAo, tex.a);

}