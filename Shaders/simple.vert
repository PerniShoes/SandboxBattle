#version 120

attribute vec2 position;  // vertex position
attribute vec2 texCoord;  // texture coordinates

varying vec2 vTexCoord;   // passed to fragment shader

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    vTexCoord = texCoord;
}
