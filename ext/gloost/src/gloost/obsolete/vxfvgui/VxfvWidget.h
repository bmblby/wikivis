
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



#ifndef H_VXFV_VXFVWIDGET
#define H_VXFV_VXFVWIDGET



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <list>
#include <memory>



namespace gloost
{

  class VxfvGui;
  class WindowWidget;
  class Mesh;
  class MessageNode;

  namespace human_input { class MouseState; }


  namespace obsolete { class Vbo; }


  //  Base class for all Widgets used by the gui

class VxfvWidget
{
	public:

    // class constructor
    VxfvWidget(VxfvGui* gui);

    // class destructor
	  virtual ~VxfvWidget();


    virtual void onInitWidget(){};
    virtual void onUpdateMesh(){};

    virtual void onEnterFrame(){};
    virtual void onExitFrame(){};

    virtual void onHandleMouse(gloost::human_input::MouseState* mouse);

    virtual void onDraw(){};


    // HANDLER FOR MOUSE STATES AND EVENTS
    virtual void onMouseEnterEvent();
    virtual void onMouseLeaveEvent();
    virtual void onMouseDownEvent();
    virtual void onMouseUpEvent();
    virtual void onMouseDownOutsideEvent();
    virtual void onMouseUpOutsideEvent();
    virtual void onMouseWithin();
    virtual void onMouseOutside();

    // HANDLER FOR KEY EVENTS
    virtual bool onHandleKey(unsigned char key, bool state);


//    virtual void onMove(float speedX, float speedY){};


    virtual void onDisable(){};
    virtual void onEnable(){};

    virtual void onHide(){};
    virtual void onUnhide(){};



    // returs true if point is within the widget
    bool inside(const gloost::Point3& location);



    // adds a quad to a mesh
    static void pushQuad( float x, float y, float z,
                          float scaleX, float scaleY, float scaleZ,
                          float texMinX, float texMinY,
                          float texScaleX, float texScaleY,
                          std::shared_ptr<gloost::Mesh> mesh);

    // adds a quad to a mesh
    static void pushQuadCenter( float x, float y, float z,
                                float scaleX, float scaleY, float scaleZ,
                                float texMinX, float texMinY,
                                float texScaleX, float texScaleY,
                                std::shared_ptr<gloost::Mesh> mesh);




    gloost::Point3  _position;
    gloost::Vector3 _scale;
    std::string     _title;
    std::string     _tooltipText;
    WindowWidget*   _window;


    int  _mouseWithin;
    bool _mouseEnterEvent;
    bool _mouseLeaveEvent;

    bool _mouseDownEvent;
    bool _mouseUpEvent;
    bool _mouseDownsOutsideEvent;
    bool _mouseUpOutsideEvent;

    bool _drag;


    gloost::vec4    _tintColor;



  protected:
    std::shared_ptr<gloost::Mesh> _mesh;
    gloost::obsolete::Vbo*  _vbo;

    VxfvGui* _gui;

    MessageNode* _messageNode;




};


} // namespace gloost


#endif // H_VXFV_VXFVWIDGET


