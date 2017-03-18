
/*
                       ___                            __
                      /\_ \                          /\ \__
                   __ \//\ \     ___     ___     ____\ \  _\
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/
                   \_/__/

                   OpenGL framework for fast demo programming

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).
*/



#ifndef H_GLOOST_VIEWPORT
#define H_GLOOST_VIEWPORT


// gloost system includes


// cpp includes
#include <GL/glew.h>


namespace gloost
{


  /// wrapper for glViewport with push/pop functionality

class Viewport
{

  public:

    Viewport(int x, int y, int width, int height);
    ~Viewport();

    // configure this viewport
    void set(int x, int y, int width, int height);

    // use this viewport
    void enter(bool push = true);

    // don't use this viewport anymore (recover old viewport if push was true)
    void leave();


    // returns the x
    int getX() const;

    // returns the y
    int getY() const;


    // returns the width
    int getWidth() const;

    // returns the height
    int getHeight() const;


  private:

    int m_vp_params[4];
    int m_old_vp_params[4];


};


} // namespace gloost

#endif // #ifndef H_GLOOST_VIEWPORT
