
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



#ifndef H_GLOOST_GUI
#define H_GLOOST_GUI


/// shows a message in the gloost::TextLog if a widget is added
// #define GLOOST_GUI_ADDWIDGET_MESSAGE_ON_TEXTLOG


// gloost system includes
#include <gloost/gui/Widget.h>
#include <gloost/gui/Slider.h>
#include <gloost/gui/Toggle.h>
#include <gloost/gui/Button.h>
#include <gloost/gui/Pad.h>
#include <gloost/gui/PropertyInspector.h>
#include <gloost/gui/TextInput.h>
#include <gloost/gui/WidgetGroup.h>
#include <gloost/gui/Histogram.h>

// gloost includes
#include <gloost/human_input/MouseState.h>

// cpp includes
#include <string>
#include <map>
#include <vector>
#include <list>



namespace gloost
{


  //  Gui holding all kinds of widgets and provides mouse events to them

class Gui
{
	public:


    typedef std::list<Widget*>         WidgetList;
    typedef WidgetList::iterator       WidgetListIterator;
    typedef WidgetList::const_iterator WidgetListConstIterator;


    /// class constructor
    Gui(const std::string& dataFolderPath = "../../data/", gloost::human_input::MouseState* mouse = 0);


    /// destructor
    virtual ~Gui();



    /// processes the gui and creates mouseevents for the current mouse state,
    /// returns 1 if the mouse was inside a widget
    bool frameStep();

    /// draw this gui
    void draw();



    /// get the node wich is graphicly located below the mouse pointer
    Widget* getWidgetByMousePosition();


    /// add a Widget to find it in the tree for communication
    unsigned int addWidget(Widget* widget);



    /// set a property on all widgets
    void setPropOnAllWidgets(std::string name, float value);

    /// set a property on all widgets
    void setPropOnAllWidgets(std::string name, std::string value);



    /// move all widgets
    void moveAllWidgets(const Vector2& offset);



    /// call mouseUpOutside on all widgets
    void callMouseUpOutsideOnAllWidgets();



    /// creates unique IDs for the Nodes
    unsigned int getNewUniqueId();



    /// get a registered Widget by Id
    Widget* getWidgetById(unsigned int id);

    /// get a registered Node by Name
    Widget* getWidgetByName(const std::string&  name);


    /// get a list of type gloost::Gui
    WidgetList* getWidgets();


    /// returns the path to folder where folders with fonts and textures for the widgets are located
    const std::string& getDataFolderPath();


    /// get a pointer to the mouse
    gloost::human_input::MouseState* getMouse();


    /// call these functions to update the gui every frame if you use glut
    void glutMouseFunc(int button, int state, int mouse_h, int screenHeightMinusMouse_v);
    void glutMotionFunc(int mouse_h, int screenHeightMinusMouse_v);


    /// send a key to all widgets
    bool handleKeyDownEvent(const unsigned char& asciiChar);


    /// enable dragMode on all widgets
    void enableDragMode();

    /// disable dragMode on all widgets
    void disableDragMode();

    /// inverse dragMode on all widgets
    void toggleDragMode();


    /// writes the gui to an xml file
    bool writeGuiToXml(const std::string& filepath);


    /// returns the current active widget (last with mouseWithin)
    Widget* getCurrentActiveWidget();

    /// set current active widget
    void setCurrentActiveWidget(Widget* widget);


    /// get last id given to a widget by the gui
    unsigned int getLastWidgetId() const;


	private:

    WidgetList     _guiElements;

    gloost::human_input::MouseState* _mouse;
    bool           _mouseIsExtern;


    unsigned int _guiWidth;
    unsigned int _guiHeight;


    /// path to the theme
    std::string _dataFolderPath;



    /// unique or default name of this node
    unsigned int _idCounter;


    /// lookup for id -> Widget
    std::map<unsigned int, Widget*> _widgetsById;

    /// lookup for name -> _Widget;
    std::map<std::string, Widget*>  _widgetsByName;

    /// current active widget
    Widget* _currentActiveWidget;


    bool _enabled;



};


} // namespace gloost


#endif // H_GLOOST_GUI




