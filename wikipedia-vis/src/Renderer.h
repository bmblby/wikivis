#ifndef H_VTA_OVERVIEW_RENDERER
#define H_VTA_OVERVIEW_RENDERER

// gloost
#include <gloost/gl/ShaderProgram.h>
#include <gloost/gl/Vbo.h>
#include <gloost/FreeTypeWriter.h>
#include <gloost/human_input/MouseState.h>

// Math glm
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// vta
#include "Model.h"


namespace vta
{

class Renderer
{
  public:

    // class constructor
    Renderer(Model&, GLfloat widht, GLfloat height);

    // initialize
    bool initialize();
    void fill_vbos();
    void display();
    void draw();
    // void draw_HUD();

    glm::vec3 screen2modelSpace(glm::vec3 pos) const;
    void resize(int width, int height);

    // update functions from controller
    void zoom(float yoffset);
    void translate(glm::vec3 vec);
    void set_mouse(gloost::human_input::MouseState mouse);
    void redraw();

    void cleanup();
///////// OLD CODE /////////////////////////////////////////////////////

    // Window size DEBUG
    int _width;
    int _height;

    float deltaTime;

  protected:
    //Handlers for buffer and shader program
    GLuint _programID;
    GLuint _vaoNode;
    GLuint _vboNode;
    GLuint _vaoEdge;
    GLuint _vboEdge;

    //Matrices
    glm::mat4 _MVP;
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _modelMatrix;

    //controls
    glm::vec3 _cameraPos;
    float _FOV;
    float _speed;
    float _mousespeed;

    float _left;
    float _right;
    float _bottom;
    float _top;
    float _near;
    float _far;

    // graph model
    Model& _model;


    gloost::Point3 _mouse_pos;
    gloost::human_input::MouseState _mouse_state;

    // gl ressources
    std::shared_ptr<gloost::gl::ShaderProgram> _edgeShader;
    std::shared_ptr<gloost::gl::ShaderProgram> _nodeShader;

    // typewriter for text rendering
    std::shared_ptr<gloost::FreeTypeWriter> _typeWriter;

    unsigned _num_nodes;
    unsigned _num_edges;
};
} // namespace vta
#endif // H_VTA_OVERVIEW_RENDERER
