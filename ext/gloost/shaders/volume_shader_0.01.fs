
/*

  author: 2008 by Felix Weissig
  mail:   thesleeper@gmx.net


*/



// surfaces texture
uniform sampler3D map_volumen;
uniform sampler2D map_transfer;
uniform float     depth_w;
uniform float     min_d;
uniform float     max_d;
uniform float     invDelta_d;
uniform float     alphaMult;


void main( void )
{
  vec3 samplePos = (gl_TextureMatrix[1]*vec4(gl_TexCoord[0].x,
                    gl_TexCoord[0].y,
                    depth_w,
                    1.0)).xyz;


  // texture color
  float density = texture3D( map_volumen, samplePos).a;

  if(density < min_d)
  {
     density = 0.0;
  }
  if(density > max_d)
  {
     density = 0.0;
  }

  float alpha = density*invDelta_d*alphaMult;
  vec4 color  = texture2D( map_transfer, vec2((density-min_d)*invDelta_d, 0));

  color = vec4(color.r, color.g, color.b, alpha*0.06);
  //vec4 color = vec4(1.0, 1.0, 0.96, alpha);


  gl_FragColor = color;
}
