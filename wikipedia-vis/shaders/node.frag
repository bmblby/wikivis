#version 430 core

///////////////////////////////////////////////////////////////////

// output
layout ( location = 0 ) out vec4 FragColor;

//////////////////////////////////////////////////////////////////
// input definitions
in vData
{
    vec4 color;
}vertex;



///////////////////////////////////////////////////////////////////


void main()
{
  FragColor = vertex.color;
  // FragColor = vec4(1.0, 0.0, 0.0, 1.0);

  // make squares round
  vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
  if (dot(circCoord, circCoord) > 1.0) {
    discard;
  }
}
