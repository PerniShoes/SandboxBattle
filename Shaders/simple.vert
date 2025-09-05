
void main()
{
    // Pass through the vertex texture coordinate
    gl_TexCoord[0] = gl_MultiTexCoord0;

    // Transform the vertex position using the default matrices
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
