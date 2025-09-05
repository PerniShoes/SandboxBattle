#version 120

uniform sampler2D uTexture;
uniform vec3 uColor;

varying vec2 vTexCoord;

void main()
{
    vec4 texColor = texture2D(uTexture, vTexCoord);
    if(texColor.r > 0.95 && texColor.g > 0.95 && texColor.b > 0.95)
        gl_FragColor = vec4(uColor, texColor.a);
    else
        gl_FragColor = texColor;
}
