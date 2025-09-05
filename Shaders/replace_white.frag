uniform sampler2D uTexture;
uniform int uNumColors;                   // number of active replacements
uniform vec4 uTargetColors[8];            // target colors
uniform vec4 uReplacementColors[8];       // replacement colors

void main()
{
    vec4 texColor = texture2D(uTexture, gl_TexCoord[0].st);
    float epsilon = 0.01;

    for(int i = 0; i < uNumColors; ++i)
    {
        if(all(lessThan(abs(texColor - uTargetColors[i]), vec4(epsilon))))
        {
            gl_FragColor = uReplacementColors[i];
            return; // stop at first match
        }
    }

    gl_FragColor = texColor; // no match, keep original
}
