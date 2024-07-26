#version 460 core

in float Height;
in vec2 coords;

layout(binding = 1) uniform sampler2D gTextureHeight0;
layout(binding = 2) uniform sampler2D gTextureHeight1;
layout(binding = 3) uniform sampler2D gTextureHeight2;
layout(binding = 4) uniform sampler2D gTextureHeight3;

uniform float gHeight0 = 16.0;
uniform float gHeight1 = 32.0;
uniform float gHeight2 = 48.0;
uniform float gHeight3 = 64.0;

out vec4 FragColor;

vec4 CalcTexColor()
{
    vec4 TexColor;

    if (Height < gHeight0) {
       TexColor = texture(gTextureHeight0, coords * 32.0f);
    } else if (Height < gHeight1) {
       vec4 Color0 = texture(gTextureHeight0, coords * 32.0f);
       vec4 Color1 = texture(gTextureHeight1, coords * 32.0f);
       float Delta = gHeight1 - gHeight0;
       float Factor = (Height - gHeight0) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight2) {
       vec4 Color0 = texture(gTextureHeight1, coords * 32.0f);
       vec4 Color1 = texture(gTextureHeight2, coords * 32.0f);
       float Delta = gHeight2 - gHeight1;
       float Factor = (Height - gHeight1) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight3) {
       vec4 Color0 = texture(gTextureHeight2, coords * 32.0f);
       vec4 Color1 = texture(gTextureHeight3, coords * 32.0f);
       float Delta = gHeight3 - gHeight2;
       float Factor = (Height - gHeight2) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else {
       TexColor = texture(gTextureHeight3, coords * 32.0f);
    }

    return TexColor;
}

void main()
{
    //float h = Height / 32.0f;
    //FragColor = vec4(h, h, h, 1.0);

    vec4 TexColor = CalcTexColor();
    FragColor = TexColor;
}