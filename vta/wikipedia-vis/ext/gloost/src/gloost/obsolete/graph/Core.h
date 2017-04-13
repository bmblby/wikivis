
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



#ifndef H_GLOOST_CORE
#define H_GLOOST_CORE



/// gloost::core types defines
#define GLOOST_CORE_ERROR            0
#define GLOOST_CORE_PURE             1
#define GLOOST_CORE_MATRIX           2
#define GLOOST_CORE_VBO              3
#define GLOOST_CORE_SHADER           4
#define GLOOST_CORE_POINTLIGHT       5
#define GLOOST_CORE_MATERIAL         6
#define GLOOST_CORE_RENDERTARGET     7
#define GLOOST_CORE_LOD              8
#define GLOOST_CORE_BILLBOARD        9
#define GLOOST_CORE_POINTCLOUD       10
#define GLOOST_CORE_COLOR            11
///#define GLOOST_CORE_...          200   <--- DON'T use number<200 for your cores!!! They are mine, MINE I TELL YOU!!!



// gloost systeme includes
#include <gloost/SharedResource.h>


// cpp includes
#include <string>


namespace gloost
{
namespace graph
{


  class Action;

  /// Base class of all Scenegraph Cores, filling the Nodes with functionality

class Core : public SharedResource
{
  public:

    Core (unsigned int typeId);
    virtual ~Core ();


    // initialise core (lazy)
    virtual void prepareCore () {};


    // traverser enters the node and handles the core (set)
    virtual void enterCore (const Action* action) {};

    // traverser enters the node and handles the core (unset)
    virtual void exitCore  (const Action* action) {};


    // returns the type id of the core
    unsigned int getType();


    // disables the functionality of the core within the scenegraph
    void setEnabled(bool enabled=true);

    // returns true if core is enabled
    bool isEnabled();


  protected:

    unsigned int _typeId;
    bool         _disabled;

};


} // namespace graph
} // namespace gloost


#endif // GLOOST_CORE_H
