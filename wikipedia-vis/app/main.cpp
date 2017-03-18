#include <stdio.h>
#include <stdlib.h>

// gl
#include <GL/glew.h>

// glfw
#include <GLFW/glfw3.h>

// cpp includes
#include <string>
#include <iostream>

#include <nanogui/nanogui.h>

// wikidb
#include "contract.h"
#include "record_not_found.h"
#include "SimPair.h"
#include "wikidb.h"

// vta
#include "Renderer.h"
#include "Model.h"
#include "Ctrl.h"
#include "Gui.h"

using namespace nanogui;

//pointer
vta::Renderer* renderer_ptr;
vta::Ctrl* ctrl_ptr;
vta::Gui* guip;
Screen *screen = nullptr;

// callback function declaration
void display(vta::Renderer& renderer);
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

 // build database
  WikiDB wikidb("/dev/shm/wiki-vis-data/pages");
  // Category one = wikidb.getCategory(1);
  // one.info();

  // Graph init
  vta::Graph g;
  vta::Model model(wikidb);
  std::string category_name = "Computer science";
  Category computer_science = wikidb.getCategoryByName(category_name);

  model.build_graph(category_name, g, 1);
  model.layout_FR();
  // model.layout_circular(2.00);

  // dump graph
  auto pos_map = get(&vta::CatProp::position, model._graph);
  dump_graph_layout("test_graph", model._graph, pos_map);

  // Dump graph in dot format for debugging
  // auto name_map = boost::get(&vta::CatProp::title, g);
  // boost::default_writer dw;
  // std::ofstream file("graph.dot");
  // if(file.is_open()) {
  //   boost::write_graphviz(file, g,
  //       boost::make_label_writer(name_map),
  //       dw, dw, get(&vta::CatProp::index, g));
  // }

  vta::Renderer renderer(model, main_window_width, main_window_height);    // new renderer instance
  renderer_ptr = &renderer;
  if (!renderer.initialize())       //load shaders from file
  {
    std::cerr << "error initializing shaders for vis" << std::endl;
    exit(EXIT_FAILURE);
  }
  renderer.resize(main_window_width, main_window_height); // initial resize

  vta::Ctrl ctrl(model, renderer);
  ctrl_ptr = &ctrl;

  glfwMakeContextCurrent(main_window);
  // gui class
  vta::Gui gui(main_window, ctrl_ptr);
  guip = &gui;
  gui.search_box(glm::vec3(10, 10, 0), 45, 25);


  // Main loop
  do{
    glfwPollEvents();

    //get deltaTime
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    renderer.deltaTime = deltaTime;

    // Main Window (Visualization)
    display(renderer);
    gui.display();

    glfwSwapBuffers(main_window);
  }
  while (glfwGetKey(main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(main_window) == 0);


  // Cleanup
  renderer.cleanup();
  glfwDestroyWindow(main_window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
//////////////////////////////////////////////////////////////////////////////

void display(vta::Renderer& renderer)
{
  renderer.display();
  // ctrl->reset_mouse_state(); // causes segmentaion fault
}

void resizefun(GLFWwindow* window, int width, int height)
{
  guip->resizefun(width, height);
  renderer_ptr->resize(width, height);
}

void mousebuttonfun(GLFWwindow* window, int button, int action, int mods)
{
  guip->mousebuttonfun(button, action, mods);

  double xpos = 0, ypos = 0;
  // retrieve latest cursor position
  glfwGetCursorPos(window, &xpos, &ypos);

  if (action)
    ctrl_ptr->mousePress(xpos, ypos, button, mods);
  else
    ctrl_ptr->mouseRelease(xpos, ypos, button, mods);
}

void scrollfun(GLFWwindow* window, double xoffset, double yoffset)
{
  guip->scrollfun(xoffset, yoffset);
  ctrl_ptr->mouseScroll((float) yoffset);

}

void cursorposfun(GLFWwindow* window, double xpos, double ypos)
{
  // if(guip->mouse_in_gui(xpos, ypos))
    guip->cursorfun(xpos, ypos);
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

//////////////////////////////////////////////////////////////////////////////

template<typename Graph, typename PositionMap>
void dump_graph_layout(std::string name, const Graph& g, PositionMap position)
{
  std::ofstream out((name + ".dot").c_str());
  out << "graph " << name << " {" << std::endl;

  typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    out << "  n" << get(&vta::CatProp::index, g, *vi) << "[ pos=\""
        << (int)position[*vi][0] << ", " << (int)position[*vi][1]
        << "\" ];\n";
  }

  typename boost::graph_traits<Graph>::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    out << "  n" << get(&vta::CatProp::index, g, source(*ei, g)) << " -- n"
        << get(&vta::CatProp::index, g, target(*ei, g)) << ";\n";
  }
  out << "}\n";
}
