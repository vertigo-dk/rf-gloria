#version 150

// this is how we receive the texture
uniform sampler2DRect xyzMap;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

uniform float verticalAdd;
uniform float horizontalAdd;

in vec2 texCoordVarying;

out vec4 outputColor;
 
void main()
{
    vec2 size = vec2(1024,768);
    vec3 coord = texture(xyzMap, texCoordVarying).xyz;
    
    vec3 c1 =  texture(tex1, coord.xy * size).rgb;
    vec3 c2 =  texture(tex2, coord.xz * size).rgb;
    
    outputColor.rgb += c1 * verticalAdd;
    outputColor.rgb += c2 * horizontalAdd;
    
    outputColor.a = 1.0;
    
}