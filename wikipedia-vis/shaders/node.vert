#version 330 core

///////////////////////////////////////////////////////////////////
// attribute layout definitions

layout (location = 0 ) in vec3 in_position;
layout (location = 1 ) in vec4 in_color;

///////////////////////////////////////////////////////////////////
// uniforms

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

// uniform mat4 MVP;

///////////////////////////////////////////////////////////////////
// output

out vData
{
    vec4 color;
}vertex;


///////////////////////////////////////////////////////////////////
// implementation

void main()
{
  gl_Position = Projection * View * Model * vec4(in_position, 1.0);
  gl_PointSize = 8.0f;
  vertex.color = in_color;
}
