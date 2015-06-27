#version 120
#extension GL_EXT_geometry_shader4 : enable

varying vec3 eyeSpaceNormal;
varying vec4 _eyeSpaceVertexPos;
varying vec4 eyeSpaceVertexPos;

void main(void)
{
    // Compute per-face normal
    vec4 d1 = gl_PositionIn[0] - gl_PositionIn[1];
    vec4 d2 = gl_PositionIn[0] - gl_PositionIn[2];
    eyeSpaceNormal = cross(d1.xyz,d2.xyz);
    eyeSpaceVertexPos = _eyeSpaceVertexPos;
    
    // Emit all input vertices
    for(int i=0; i< gl_VerticesIn; i++){
        gl_Position = gl_PositionIn[i];
        gl_TexCoord[0] = gl_TexCoordIn[i][0];
        gl_TexCoord[1] = gl_TexCoordIn[i][1];
        gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex();
    }
    EndPrimitive();
}