
/*

author: 2008 by Stephan Beck
mail:   stephan@pixelstars.de

*/
uniform sampler2D       color_map;
uniform sampler2DShadow shadow_map;
varying vec3 vPosition;
varying vec3 vNormal;

void main(void)
{

  vec4  color  = texture2D(color_map, gl_TexCoord[0].st);
  float shadow = shadow2DProj(shadow_map, gl_TexCoord[1]).z;

  float lightness = (color.r*0.3 + color.g*0.59 + color.b*0.11);

  vec3 vNormal   = normalize(vNormal);
  vec3 NEyeVec   = normalize(-vPosition);

  vec4 cAmbient   = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 cDiffuse   = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 cSpecColor = vec4(0.0, 0.0, 0.0, 1.0);


  vec3 NLightVec = vec3(gl_LightSource[0].position.xyz) - vPosition;
  NLightVec      = normalize(NLightVec);
  vec3 NHalfVec  = normalize(NLightVec + NEyeVec);


  cAmbient = gl_LightSource[0].ambient * color;

  float ndotl = max( dot(NLightVec,vNormal), 0.0 );
  cDiffuse    = gl_LightSource[0].diffuse * color * ndotl;

  cSpecColor += gl_LightSource[0].specular * pow(max( dot(vNormal, NHalfVec), 0.0 ), 80.0);

  gl_FragColor = gl_Color * (cAmbient + shadow * (cDiffuse + (cSpecColor*4.8*lightness)));
}
