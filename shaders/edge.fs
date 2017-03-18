#version 330 core

///////////////////////////////////////////////////////////////////
// layout definitions

layout ( location = 0 ) out vec4 FragColor;


///////////////////////////////////////////////////////////////////
// input/output definitions

in vec4 color;

///////////////////////////////////////////////////////////////////
// implementation

void main()
{
  FragColor = color;
  //FragColor = vec4(0.0, 0.9, 0.0, 0.5);
}
