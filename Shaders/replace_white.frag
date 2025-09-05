#version 120

uniform sampler2D uTexture;
uniform vec4 uTargetColor;      // Color to replace
uniform vec4 uReplacementColor; // Color to replace with

varying vec2 vTexCoord;

void main()
{
    vec4 texColor = texture2D(uTexture, vTexCoord);

    float epsilon = 0.01; // tolerance for floating point comparison
    if (all(lessThan(abs(texColor - uTargetColor), vec4(epsilon))))
        gl_FragColor = uReplacementColor;
    else
        gl_FragColor = texColor;
}
