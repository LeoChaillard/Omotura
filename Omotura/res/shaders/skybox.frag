#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, texCoords);
    //FragColor.rgb = texture(cubeMap, TexCoords).rgb;
    //FragColor.rgb *= 0.3;
    //FragColor.g *= 0.5;
    //FragColor.b *= 0.1;
    //FragColor.a = 1.0;
}