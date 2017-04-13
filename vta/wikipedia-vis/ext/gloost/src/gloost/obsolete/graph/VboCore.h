
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



#ifndef H_GLOOST_VBOCORE
#define H_GLOOST_VBOCORE



// gloost system includes
#include <gloost/graph/Core.h>
#include <gloost/obsolete/Vbo.h>
#include <gloost/BitMask.h>


#define GLOOST_VBOCORE_NOTHING       0
#define GLOOST_VBOCORE_BIND          1
#define GLOOST_VBOCORE_UNBIND        2
#define GLOOST_VBOCORE_BIND_AND_DRAW 3
#define GLOOST_VBOCORE_DRAW          4


namespace gloost
{

class Material;

namespace graph
{



class Action;


 /// Binds, Draws or unbinds a gloost::obsolete::Vbo

class VboCore : public Core
{

	public:

		 VboCore (obsolete::Vbo* vbo,
              unsigned int coreEnterMode = GLOOST_VBOCORE_BIND_AND_DRAW,
              unsigned int coreExitMode  = GLOOST_VBOCORE_UNBIND);


		~VboCore ();



		/* virtual */ void enterCore (const Action* action);
		/* virtual */ void exitCore  (const Action* action);




    // get the Vbo
		obsolete::Vbo* getVbo();

		// set a vbo
		void setVbo(obsolete::Vbo* g);

		// sets behavior of the VboCore if enterCore() is called
		void setCoreEnterMode(unsigned int mode);

		// sets behavior of the VboCore if exitCore() is called
		void setCoreExitMode(unsigned int mode);



    VboCore* operator= (Core*);



  private:

    obsolete::Vbo* _vbo;

    unsigned int _coreEnterMode;
    unsigned int _coreExitMode;

};

} // namespace graph
} // namespace gloost

#endif // H_GLOOST_VBOCORE
