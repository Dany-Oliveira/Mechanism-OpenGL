#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 colorKey; //color to remove
uniform bool useColorKey;
uniform vec3 rectColor;  
uniform bool useRectColor;  
void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    
    // If the pixel is magenta discard
    if (useColorKey && texColor.rgb == colorKey)
    {
        discard; // Discard the fragment, making it fully transparent
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