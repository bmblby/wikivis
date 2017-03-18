#version 120


uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

varying vec2 texCoord;

void main(void)
{
  gl_Position = Projection * View * Model * gl_Vertex;
  texCoord    = gl_MultiTexCoord0.xy;
}
