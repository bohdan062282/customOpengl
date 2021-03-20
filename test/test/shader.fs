#version 330 core

out vec4 FragColor;

in vec3 outPos;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    //FragColor = texture(texture0, texCoord);
    FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 1.0);
}