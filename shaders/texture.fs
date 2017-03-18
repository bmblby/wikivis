#version 130

uniform sampler2D textureID;


in vec2 texCoord;

out vec4 out_fragColor;


///////////////////////////////////////////////////////////////////////////////


void main(void)
{
  out_fragColor = texture2D(textureID, texCoord);
}
