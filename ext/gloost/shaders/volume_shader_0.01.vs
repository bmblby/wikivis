
/*

  author: 2008 by Felix Weissig
  mail:   thesleeper@gmx.net

  This shader renders just texture with an defined akpha value.
*/


void main( void )
{
   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_TexCoord[1] = gl_MultiTexCoord1;
}
