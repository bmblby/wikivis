
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



#ifndef H_VXFV_VXFVGUI
#define H_VXFV_VXFVGUI



// vxfv includes


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <map>
#include <list>
#include <vector>



#define VXFV_VXFVGUI_WRITER_WINDOWBORDER 0
#define VXFV_VXFVGUI_WRITER_BUTTONS      1
#define VXFV_VXFVGUI_WRITER_SMALL_VALUES 2




namespace gloost
{
  class MouseState;
  class FreeTypeWriter;
  class VxfvWidget;
  class WindowWidget;

  namespace human_input { class MouseState; }


  //  VxfvGui creates and handles VxfvWidgets

class VxfvGui
{
	public:


	  typedef std::map<std::string, WindowWidget*>::iterator  WindowMapIterator;
	  typedef std::map<std::string, VxfvWidget*>::iterator    WidgetMapIterator;
	  typedef std::list<WindowWidget*>::iterator              WindowListIterator;



    // class constructor
    VxfvGui(const std::string& dataFolderPath);

    // class destructor
	  ~VxfvGui();


    // returns the file path of the data folder
	  const std::string& getDataFolderPath();


    // init the VxfvGui
	  void init();

    // process events
	  void frameStep();


	  // mouse motion callback
	  void mouseMotionFunc(int mouse_h, int screenHeightMinusMouse_v);

	  // mouse callback
	  void mouseFunc(int button, int state, int mouse_h, int screenHeightMinusMouse_v);

	  // key press callback
	  bool keyFunc(unsigned char key, bool state = true);



    // draw gui
    void draw();



    // creates a Window widget
    WindowWidget* createWindow(const std::string& name);

    // adds a Window widget or a derivative to the gui
    WindowWidget* addWindow(const std::string& name, WindowWidget* window);

    // returns a window widget for a name
    WindowWidget* getWindow(const std::string& name);


    // adds widget
    void addWidget();



    // returns the gloost::human_input::MouseState used by the Gui
    const gloost::human_input::MouseState* getMouse() const;
    gloost::human_input::MouseState* getMouse();


    // returns a screen writer
    gloost::FreeTypeWriter* getScreenWriter(unsigned id);


    // registers a widget and returns an id to reference it
    unsigned registerWidget(VxfvWidget* widget);


    // returns true is mouse was handled by the gui
    bool wasGuiUsed();



    unsigned _atlasTextureId;

    float _guiTime;
    bool  _disabled;

	protected:

    std::string _dataFolderPath;


    std::map<unsigned, VxfvWidget*>      _widgetsById;
    unsigned                             _widgetIdCounter;


    std::map<std::string, WindowWidget*> _windows;

    std::list<WindowWidget*>             _windowDrawList;

    bool                                 _guiWasUsed;

    gloost::human_input::MouseState*                       _mouse;

    std::vector<gloost::FreeTypeWriter*> _screenWriters;

};


} // namespace gloost


#endif // H_VXFV_VXFVGUI


