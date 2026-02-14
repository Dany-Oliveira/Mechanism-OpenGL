#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 colorKey;
uniform bool useColorKey;
uniform vec3 rectColor;  
uniform bool useRectColor;  
void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    
    
    if (useColorKey && texColor.rgb == colorKey)
    {
        discard;
    }
    
  
    if (useRectColor)
    {
        FragColor = vec4(rectColor, texColor.a);
    }
    else
    {
        FragColor = texColor;
    }
}