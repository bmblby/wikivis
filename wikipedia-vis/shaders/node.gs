#version 330

///////////////////////////////////////////////////////////////////

// vertex attribute layout
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

///////////////////////////////////////////////////////////////////

// input/output definitions

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

// uniform mat4 MVP;

// Color input
in vData
{
    vec4 color;
}vertices[];


// Color output
out fData
{
    vec4 color;
}frag;

///////////////////////////////////////////////////////////////////

// implementation

void main(void)
{
  vec4 pos = gl_in[0].gl_Position;

  // pos = MVP * pos;
  pos = Projection * View * Model * pos;

  float node_size = Model[0][0] * 1.1;

  if (node_size > 0.02)
    node_size = 0.02;

  vec4 red  = vec4(1.0, 0.0, 0.0, 1.0);
  vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
//
//  gl_Position = pos;
//  frag.color = red;
//  EmitVertex();
//
//  gl_Position = pos + vec4(-node_size, node_size, 0.0, 0.0);
//  frag.color = red;
//  EmitVertex();
//
//  gl_Position = pos + vec4(node_size, node_size, 0.0, 0.0);
//  frag.color = blue;
//  EmitVertex();
//
//  gl_Position = pos + vec4(node_size, -node_size, 0.0, 0.0);
//  frag.color = blue;
//  EmitVertex();


  gl_Position = pos + vec4(-node_size, node_size, 0.0, 0.0);
  frag.color = vertices[0].color;
  EmitVertex();

  gl_Position = pos + vec4(-node_size, -node_size, 0.0, 0.0);
  frag.color = vertices[0].color;
  EmitVertex();

  gl_Position = pos + vec4(node_size, node_size, 0.0, 0.0);
  frag.color = vertices[0].color;
  EmitVertex();

  gl_Position = pos + vec4(node_size, -node_size, 0.0, 0.0);
  frag.color = vertices[0].color;
  EmitVertex();

  EndPrimitive();

}
