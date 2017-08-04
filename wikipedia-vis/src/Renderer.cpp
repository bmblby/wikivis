#include "Renderer.h"

namespace vta
{


Renderer::Renderer(Model& model, GLfloat width, GLfloat height):

  _width(width),
  _height(height),

  _speed(2.0f),
  _mousespeed(0.0009f),

  _cameraPos(0.0f, 0.0f, -3.0f),
  _FOV(45.0f),

  // ortho projection
  _left(0),
  _right(_width),
  _bottom(_height),
  _top(0),
  // _left(-1.0f),
  // _right(1.0f),
  // _bottom(-1.0f),
  // _top(1.0f),
  _near(0.1f),
  _far(10.0f),

  _model(model),
  _mouse_pos(0.0, 0.0, 0.0),

  _edgeShader(nullptr),
  _nodeShader(nullptr),
  _typeWriter(nullptr),

  _num_nodes(0),
  _num_edges(0)
{
  // create TypeWriter
  _typeWriter = gloost::FreeTypeWriter::create("../../res/fonts/Verdana.ttf", 23.0f, true);


  // initalize Model View Projection

  // perspective view
  _projectionMatrix = glm::perspective(
    glm::radians(_FOV),
    (GLfloat)_width / (GLfloat)_height,
    _near, _far
  );

  // orthographic view
  // _projectionMatrix = glm::ortho(
  //   _left,
  //   _right,
  //   _bottom,
  //   _top,
  //   _near,
  //   _far
  // );

  _viewMatrix = glm::lookAt(
    _cameraPos,
    glm::vec3(0, 0, 0), // direction where the camera is looking
    glm::vec3(0, 1, 0)  // head is up (0,-1,0) is upsidedown
  );

  // translation for orthographic projection
  // _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(_width/2, _height/2, 0.0f));
  // float scale(100.0f);
  // _modelMatrix = glm::scale(_modelMatrix, glm::vec3(scale, scale, 0.0f));

  _modelMatrix = glm::mat4(1.0f);
  _MVP = _projectionMatrix * _viewMatrix * _modelMatrix;
}

bool
Renderer::initialize()
{
  std::string edge_v = "../../shaders/edge.vert";
  std::string edge_f = "../../shaders/edge.frag";

  // create shader program and attach all components
  _edgeShader = gloost::gl::ShaderProgram::create();
  _edgeShader->attachShaderFile(GLOOST_SHADERPROGRAM_VERTEX_SHADER, edge_v);
  _edgeShader->attachShaderFile(GLOOST_SHADERPROGRAM_FRAGMENT_SHADER, edge_f);

  std::string node_v = "../../shaders/node.vert";
  std::string node_f = "../../shaders/node.frag";
  std::string node_g = "../../shaders/node.gs";

  // create shader program and attach all components
  _nodeShader = gloost::gl::ShaderProgram::create();
  _nodeShader->attachShaderFile(GLOOST_SHADERPROGRAM_VERTEX_SHADER,   node_v);
  _nodeShader->attachShaderFile(GLOOST_SHADERPROGRAM_FRAGMENT_SHADER, node_f);
  // _nodeShader->attachShaderFile(GLOOST_SHADERPROGRAM_GEOMETRY_SHADER, node_g);

  // gen VAOs and VBOs
  // _programID = LoadShaders( "SimpleTransform.vertexshader", "SimpleColor.fragmentshader" );
  glGenVertexArrays(1, &_vaoNode);
  glGenBuffers(1, &_vboNode);
  glGenVertexArrays(1, &_vaoEdge);
  glGenBuffers(1, &_vboEdge);

  return true;
}

void
Renderer::fill_vbos()
{
  //bind node buffer and push to gpu
  auto nodes = _model.get_nodes();
  _num_nodes = nodes.size();
  glBindBuffer(GL_ARRAY_BUFFER, _vboNode);
  glBufferData(
    GL_ARRAY_BUFFER,
    //pos=3 and color=4 => 7
    nodes.size() * (sizeof(float) * 7),
    nodes.data(),
    GL_STATIC_DRAW
  );

  //bind edge buffer and push to gpu
  auto edges = _model.get_edges();
  _num_edges = edges.size();
  glBindBuffer(GL_ARRAY_BUFFER, _vboEdge);
  glBufferData(
    GL_ARRAY_BUFFER,
    //2 times pos=3 and color=4 => 14
    edges.size() * (sizeof(float) * 14),
    0,
    GL_STATIC_DRAW
  );

  std::vector<glm::vec3> edgePos;
  std::vector<std::array<float, 4>> edgeCol;
  for (auto i : edges) {
    edgePos.push_back(std::get<0>(i));
    edgePos.push_back(std::get<1>(i));
    edgeCol.push_back(std::get<2>(i));
    edgeCol.push_back(std::get<2>(i));
  };

  glBufferSubData(
    GL_ARRAY_BUFFER,
    0,
    edgePos.size() * sizeof(GLfloat) * 3,
    edgePos.data()
  );

  glBufferSubData(
    GL_ARRAY_BUFFER,
    edgePos.size() * sizeof(GLfloat) * 3,
    edgeCol.size() * sizeof(GLfloat) * 4,
    edgeCol.data()
  );
}

void
Renderer::display()
{
  if (_model._dirty)
  {
    fill_vbos();
    _model._dirty = false;
  }
    fill_vbos();

  // // setup clear color and clear screen
  glClearColor(0.059f, 0.176f, 0.251f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // // apply viewport
  glViewport(0, 0, _width, _height);

  // enalbe alphe values for drawing
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_PROGRAM_POINT_SIZE);

  // Draw nodes and edges
  draw();
}

void
Renderer::draw()
{
    auto nodes = _model.get_nodes();
    size_t point_size = sizeof(nodes[0].first);
    size_t color_size = sizeof(nodes[0].second);
    size_t total_size = point_size + color_size;

    // draw Nodes
    glBindVertexArray(_vaoNode);
    glUseProgram(_nodeShader->getHandle());
    // GLuint MatrixID = glGetUniformLocation(_nodeShader->getHandle(), "MVP");
    GLuint projectionID = glGetUniformLocation(_nodeShader->getHandle(), "Projection");
    GLuint viewID = glGetUniformLocation(_nodeShader->getHandle(), "View");
    GLuint modelID = glGetUniformLocation(_nodeShader->getHandle(), "Model");

    // _MVP = _projectionMatrix * _viewMatrix * _modelMatrix;
    // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &_MVP[0][0]);
    glUniformMatrix4fv(projectionID , 1, GL_FALSE, &_projectionMatrix[0][0]);
    glUniformMatrix4fv(viewID , 1, GL_FALSE, &_viewMatrix[0][0]);
    glUniformMatrix4fv(modelID , 1, GL_FALSE, &_modelMatrix[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vboNode);
    glVertexAttribPointer(
        0,
        point_size / sizeof(float),
        GL_FLOAT,
        GL_FALSE,
        total_size,
        (void*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        color_size / sizeof(float),
        GL_FLOAT,
        GL_FALSE,
        total_size,
        (void*)(point_size)
    );
    glDrawArrays(GL_POINTS, 0, _num_nodes);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


    // draw Edges
    glBindVertexArray(_vaoEdge);
    glUseProgram(_edgeShader->getHandle());
    // GLuint MatrixID = glGetUniformLocation(_nodeShader->getHandle(), "MVP");
    projectionID = glGetUniformLocation(_edgeShader->getHandle(), "Projection");
    viewID = glGetUniformLocation(_edgeShader->getHandle(), "View");
    modelID = glGetUniformLocation(_edgeShader->getHandle(), "Model");

    glUniformMatrix4fv(projectionID , 1, GL_FALSE, &_projectionMatrix[0][0]);
    glUniformMatrix4fv(viewID , 1, GL_FALSE, &_viewMatrix[0][0]);
    glUniformMatrix4fv(modelID , 1, GL_FALSE, &_modelMatrix[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vboEdge);
    glVertexAttribPointer(
        0,
        point_size / sizeof(float),
        GL_FLOAT,
        GL_FALSE,
        point_size,
        (void*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        color_size / sizeof(float),
        GL_FLOAT,
        GL_FALSE,
        color_size,
        (void*)(point_size * 2 * _num_edges)
    );

    // two vertices for every edge
    glDrawArrays(GL_LINES, 0,  _num_edges * 2);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

glm::vec3
Renderer::screen2modelSpace(glm::vec3 pos) const
{
    GLfloat win_z;
    glReadPixels(pos[0], _height - pos[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
    // subtract height diffrent origin in glfw then in opengl
    glm::vec3 screen_pos(pos[0], _height -pos[1], win_z);
    // std::cout << "screen space position: (" << glm::to_string(screen_pos) << ")\n";

    //unproject screen space
    glm::vec3 vec = glm::unProject(screen_pos, _viewMatrix * _modelMatrix, _projectionMatrix, glm::vec4(0.0f, 0.0f, _width, _height));
    // std::cout << "glm::unprojec to mouse position: " << glm::to_string(vec) << "\n\n";
    return vec;
}

void
Renderer::resize(int width, int height)
{
  _width  = width;
  _height = height;

  //resize gl Viewport
  glViewport(0, 0, _width, _height);
  redraw();
//
//   glMatrixMode(GL_PROJECTION); // modify the projection matrix
//   glLoadIdentity();            // load an identity matrix into the projection matrix
//   // glOrtho(0, width, height, 0, 0.1f, 100.0f); // create new projection matrix
//   // glOrtho(0, width, 0, height, -1.0f, 1.0f); // create new projection matrix
//
//   /// Important!!! You need to switch back to the model-view matrix
//   /// or else your OpenGL calls are modifying the projection matrix!
//   glMatrixMode(GL_MODELVIEW); // return to the model matrix
//   glLoadIdentity();
}

/// Update view from ctrl
void
Renderer::zoomFOV(float yoffset)
{
    // zoom for perspective View
    // std::cout << "yoffset: " << yoffset << std::endl;
    if(_FOV >= 1.0f && _FOV <= 90.0f)
        _FOV -= yoffset;
    if(_FOV <= 1.0f)
        _FOV = 1.0f;
    if(_FOV >= 90.0f)
        _FOV = 90.0f;
    // std::cout << "FOV: " << _FOV << "\n";
    _projectionMatrix = glm::perspective(
        glm::radians(_FOV),
        (GLfloat)_width / (GLfloat)_height,
        0.1f, 100.0f
    );
}

void
Renderer::zoom(float yoffset)
{
    //ortho zoom
    float scale(0.0f);
    if(yoffset < 0)
        scale = 0.9f;
    else
        scale = 1.1f;
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(scale, scale, 0.0f));

}

void
Renderer::translate(glm::vec3 vec)
{
    vec = glm::vec3(vec.x /1000, vec.y/1000, 0.0);
    _modelMatrix = glm::translate(glm::mat4(1.0f), vec);
    //debug
    // std::cout << "ModelMatrix: " << glm::to_string(_modelMatrix)<< "\n ";

}

void
Renderer::set_mouse(gloost::human_input::MouseState mouse)
{
  _mouse = mouse;
  _mouse_pos = mouse.getPosition();
}

void
Renderer::redraw()
{
  // Fill vbos with new positions
  fill_vbos();

  // Reload shaders
  _edgeShader->reloadShaders();
  _nodeShader->reloadShaders();

  _projectionMatrix = glm::perspective(
    glm::radians(_FOV),
    (GLfloat)_width / (GLfloat)_height,
    _near, _far
  );

  std::cout << "Reload shaders" << std::endl;
}

void
Renderer::cleanup()
{
    glDeleteBuffers(1, &_vboNode);
    glDeleteProgram(_programID);
    glDeleteVertexArrays(1, &_vaoNode);
}

} // namespace vta
