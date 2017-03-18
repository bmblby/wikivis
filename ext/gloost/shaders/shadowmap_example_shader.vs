
/*

author: 2008 by Stephan Beck
mail:   stephan@pixelstars.de

*/


varying vec3 vPosition;
varying vec3 vNormal;

void main(void)
{

  gl_Position = ftransform();

  gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;

  vPosition     = (gl_ClipVertex / gl_ClipVertex.w).xyz;

  gl_TexCoord[0].st = gl_MultiTexCoord0.xy;
  gl_TexCoord[1] = gl_TextureMatrix[1]*gl_ClipVertex;

  gl_FrontColor = gl_Color;

  // Eye-space lighting
  vNormal = gl_NormalMatrix * gl_Normal;
}
