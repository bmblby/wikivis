// cpp includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <nanogui/nanogui.h>

// wikidb
#include "contract.h"
#include "record_not_found.h"
#include "SimPair.h"
#include "wikidb.h"

// vta
#include "Renderer.h"
#include "Model.h"
#include "Controller.h"
#include "Gui.h"
#include "View.h"

using namespace nanogui;

//pointer
vta::Renderer* renderer_ptr;
vta::Controller* ctrl_ptr;
vta::Gui* guip;
Screen *screen = nullptr;

// callback function declaration
void resizefun(GLFWwindow* window, int width, int height);
void mousebuttonfun(GLFWwindow* window, int button, int action, int mods);
void scrollfun(GLFWwindow* window, double xoffset, double yoffset);
void cursorposfun(GLFWwindow* window, double xpos, double ypos);
void keyfun(GLFWwindow* window, int key, int scancode, int action, int mods);
void charfun(GLFWwindow* window, unsigned int codepoint);
void glfw_errorfun(int error, const char* description);

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

template<typename Graph, typename PositionMap>
void dump_graph_layout(std::string name, const Graph& g, PositionMap position);


int main(int argc, char *argv[])
{
  // GLFW window layout
  #define nowall

  // // GLFW
  if (!glfwInit())
      exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  // GLFW main window init
  #ifdef wall
  int main_window_width = 3840;
  int main_window_height = 2130;
  GLFWwindow* main_window = glfwCreateWindow(main_window_width, main_window_height  * 2,
                                "VisualTextAnalytics", NULL, NULL);
  #else
  int main_window_width = 1024 ;
  int main_window_height = 768;
  GLFWwindow* main_window = glfwCreateWindow(main_window_width, main_window_height,
                                "VisualTextAnalytics", NULL, NULL);
  #endif
  if (main_window == nullptr)
  {
    glfwTerminate();
    std::cerr << "Failed to create glfwWindow. " << std::endl;
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(main_window, 0, 0);
  glfwGetFramebufferSize(main_window, &main_window_width, &main_window_height);

  //callback functions for glfw main window
  glfwMakeContextCurrent(main_window);
  glfwSetFramebufferSizeCallback(main_window, resizefun);
  glfwSetMouseButtonCallback(main_window, mousebuttonfun);
  glfwSetScrollCallback(main_window, scrollfun);
  glfwSetCursorPosCallback(main_window, cursorposfun);
  glfwSetKeyCallback(main_window, keyfun);
  glfwSetCharCallback(main_window, charfun);
  glfwSetErrorCallback(glfw_errorfun);

  glewExperimental = true;
  if (GLEW_OK != glewInit())
    {
      std::cerr << "'glewInit()' failed." << std::endl;
      exit(EXIT_FAILURE);
  }
  // used to catch escape key
  glfwSetInputMode(main_window, GLFW_STICKY_KEYS, GL_TRUE);


  // WikiDB wikidb("/dev/shm/wiki-vis-data/pages");
  WikiDB wikidb("/media/HDD2/data/database/enwiki2016no-comp");


  // Graph init
  vta::Model model(wikidb);
  // Category computer_science = wikidb.getCategoryByName("Computer science");
  // vta::Graph g = model.graph(computer_science, 2);
  Category main_topic_rev = wikidb.getCategoryByName("Main topic classifications");
  size_t depth = 2;

  model.initIDDFS(main_topic_rev, depth);
  model.layout(main_topic_rev, main_window_width, main_window_height, depth);

  // auto fr_map = model.layout_circular(1.0);
  // model.write_layout(fr_map);

  // dump graph layout to file
  // auto pos_map = get(&vta::CatProp::position, model._graph);
  // model.dump_graph(g, "test_dump");

  vta::Renderer renderer(model, main_window_width, main_window_height);
  renderer_ptr = &renderer;
  if (!renderer.initialize())
  {
    //load shaders from file
    std::cerr << "error initializing shaders for vis" << std::endl;
    exit(EXIT_FAILURE);
  }
  renderer.resize(main_window_width, main_window_height); // initial resize


  glfwMakeContextCurrent(main_window);
  vta::Gui gui(main_window, model);
  guip = &gui;
  gui.search_box(glm::vec3(10, 10, 0), 45, 25);

  vta::View view(model, main_window);
  // view.drawHomeView();

  vta::Controller ctrl(model, renderer, view, gui);
  ctrl_ptr = &ctrl;

  // Main loop
  do{
    glfwPollEvents();

    //get deltaTime
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    renderer.deltaTime = deltaTime;
    glClearColor(0.059f, 0.176f, 0.251f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Main Window (Visualization)
    renderer.display();
    // view.beginFrame();
    // view.drawBubble();
    // view.endFrame();

    gui.display();

    glfwSwapBuffers(main_window);
  }
  while (glfwGetKey(main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(main_window) == 0);

  // Cleanup
  view.cleanup();
  renderer.cleanup();
  glfwDestroyWindow(main_window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
//////////////////////////////////////////////////////////////////////////////

void resizefun(GLFWwindow* window, int width, int height)
{
  guip->resizefun(width, height);
  renderer_ptr->resize(width, height);
}

void mousebuttonfun(GLFWwindow* window, int button, int action, int mods)
{
  double xpos = 0, ypos = 0;
  glfwGetCursorPos(window, &xpos, &ypos);
  if(guip->contains(xpos, ypos))
    guip->mousebuttonfun(button, action, mods);

  if (action)
    ctrl_ptr->mousePress(xpos, ypos, button, mods);
  else
    ctrl_ptr->mouseRelease(xpos, ypos, button, mods);
}

void scrollfun(GLFWwindow* window, double xoffset, double yoffset)
{
  double xpos = 0, ypos = 0;
  glfwGetCursorPos(window, &xpos, &ypos);
  if(guip->contains(xpos, ypos))
    guip->scrollfun(xoffset, yoffset);
  else
    ctrl_ptr->mouseScroll((float) yoffset);
}

void cursorposfun(GLFWwindow* window, double xpos, double ypos)
{
  // if(guip->contains(xpos, ypos))
  //   guip->cursorfun(xpos, ypos);
  // else {
    int state_left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    ctrl_ptr->mouseMove(xpos, ypos, state_left);
  // }
}

void keyfun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  guip->keyfun(key, scancode, action, mods);
  // default key function
  if (action == GLFW_PRESS)
    ctrl_ptr->keyPress(key, mods);
  else // GLFW_RELEASE
    ctrl_ptr->keyRelease(key, mods);
}

void
charfun(GLFWwindow* window, unsigned int codepoint)
{
  guip->charfun(codepoint);
}

void glfw_errorfun(int error, const char* description)
{
  fputs(description, stderr);
}
