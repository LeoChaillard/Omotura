#shader vertex
#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTex;
}

#shader tess control
#version 420 core

layout(vertices=4) out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in vec2 TexCoord[];
out vec2 TextureCoord[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    if(gl_InvocationID == 0)
    {
        const int MIN_TESS_LEVEL = 4;
        const int MAX_TESS_LEVEL = 64;
        const float MIN_DISTANCE = 20;
        const float MAX_DISTANCE = 800;

        vec4 eyeSpacePos00 = viewMatrix * modelMatrix * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = viewMatrix * modelMatrix * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = viewMatrix * modelMatrix * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = viewMatrix * modelMatrix * gl_in[3].gl_Position;

        // "distance" from camera scaled between 0 and 1
        float distance00 = clamp( (abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance01 = clamp( (abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance10 = clamp( (abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance11 = clamp( (abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );

        float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
        float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
        float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
        float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}

#shader tess evaluation
#version 420 core

layout(quads, fractional_odd_spacing, ccw) in;

layout(binding = 0) uniform sampler2D heightMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec2 TextureCoord[];

out vec2 coords;
out float Height;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;

    Height = texture(heightMap, texCoord).r * 64.0f;
    coords = texCoord;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;
    p.y = Height;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * p;
}

#shader fragment
#version 420 core

in float Height;
in vec2 coords;

layout(binding = 1) uniform sampler2D gTextureHeight1;
layout(binding = 2) uniform sampler2D gTextureHeight2;
layout(binding = 3) uniform sampler2D gTextureHeight3;
layout(binding = 4) uniform sampler2D gTextureHeight4;

uniform float gHeight0 = 16.0;
uniform float gHeight1 = 32.0;
uniform float gHeight2 = 48.0;
uniform float gHeight3 = 64.0;

out vec4 FragColor;

vec4 CalcTexColor()
{
    vec4 TexColor;

    if (Height < gHeight0) {
       TexColor = texture(gTextureHeight1, coords * 32.0f);
    } else if (Height < gHeight1) {
       vec4 Color0 = texture(gTextureHeight1, coords * 32.0f);
       vec4 Color1 = texture(gTextureHeight2, coords * 32.0f);
       float Delta = gHeight1 - gHeight0;
       float Factor = (Height - gHeight0) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight2) {
       vec4 Color0 = texture(gTextureHeight2, coords * 32.0f);
       vec4 Color1 = texture(gTextureHeight3, coords * 32.0f);
       float Delta = gHeight2 - gHeight1;
       float Factor = (Height - gHeight1) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight3) {
       vec4 Color0 = texture(gTextureHeight3, coords * 32.0f);
       vec4 Color1 = texture(gTextureHeight4, coords * 32.0f);
       float Delta = gHeight3 - gHeight2;
       float Factor = (Height - gHeight2) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else {
       TexColor = texture(gTextureHeight4, coords * 32.0f);
    }

    return TexColor;
}

void main()
{
    //float h = Height / 32.0f;
    //FragColor = vec4(h, h, h, 1.0);

   vec4 TexColor = CalcTexColor();
   FragColor = TexColor;

   //FragColor = texture(gTextureHeight0, coords * 32.0f);
}