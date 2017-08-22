// cpp includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>

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
vta::View* view_ptr;
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
  int main_width = 3840;
  int main_height = 2130;
  GLFWwindow* main_window = glfwCreateWindow(main_width, main_height  * 2,
                                "VisualTextAnalytics", NULL, NULL);
  #else
  int main_width = 3840/2 ;
  int main_height = 2139;
  GLFWwindow* main_window = glfwCreateWindow(main_width, main_height,
                                "VisualTextAnalytics", NULL, NULL);
  #endif
  if (main_window == nullptr)
  {
    glfwTerminate();
    std::cerr << "Failed to create glfwWindow. " << std::endl;
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(main_window, main_width, 0);
  glfwGetFramebufferSize(main_window, &main_width, &main_height);

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

  auto t1  = std::chrono::high_resolution_clock::now();
  // WikiDB wikidb("/dev/shm/wiki-vis/enwiki2016no-comp");
  WikiDB wikidb("/dev/shm/wiki-vis/enwiki2016-02-comp");
  // WikiDB wikidb("/dev/shm/wiki-vis/enwiki2016-full");

  // Graph init
  vta::Model model(wikidb);
  Category computer_science = wikidb.getCategoryByName("Computer science");
  size_t depth = 1;
  float radius = 0.2f;

  auto t2  = std::chrono::high_resolution_clock::now();
  model.initIDDFS(computer_science , depth);
  auto t3  = std::chrono::high_resolution_clock::now();
  model.layout(computer_science , main_width, main_height, depth, radius);
  auto t4  = std::chrono::high_resolution_clock::now();

  //time plot
  auto duration_load_db = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
  auto duration_build_graph = std::chrono::duration_cast<std::chrono::seconds>(t3 - t2);
  auto duration_layout_graph = std::chrono::duration_cast<std::chrono::seconds>(t4 - t3);
  std::cout << "load Database took: " << duration_load_db.count()
  << "\nbuild Graph took: " << duration_build_graph.count()
  << "\nlayout Graph took: " << duration_layout_graph.count() << std::endl;

  // dump graph layout to file
  model.dump_graph("init_dump");

  vta::Renderer renderer(model, main_width, main_height);
  renderer_ptr = &renderer;
  if (!renderer.initialize())
  {
    //load shaders from file
    std::cerr << "error initializing shaders for vis" << std::endl;
    exit(EXIT_FAILURE);
  }
  renderer.resize(main_width, main_height); // initial resize


  glfwMakeContextCurrent(main_window);
  vta::Gui gui(main_window, model);
  guip = &gui;
  gui.search_box(glm::vec3(main_width-370, 10, 0), 45, 25);
  gui.slider_threshold(glm::vec3(main_width-220, 50, 0), 200);

  vta::View view(model, main_window,
    renderer._modelMatrix,
    renderer._viewMatrix,
    renderer._projectionMatrix);
  view_ptr = &view;

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

    // Main Window (Visualization)
    renderer.display();
    view.beginFrame();
    view.label_free_tree();
    view.HUD();
    view.endFrame();
    gui.display();

    glfwSwapBuffers(main_window);
  }
  while (glfwGetKey(main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(main_window) == 0);

  model.dump_graph("closing_dump");
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
  view_ptr->resize();
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
