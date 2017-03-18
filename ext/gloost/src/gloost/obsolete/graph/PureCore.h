
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



#ifndef GLOOST_PURECORE_H
#define GLOOST_PURECORE_H


#include <gloost/graph/Core.h>


namespace gloost
{
namespace graph
{



class Action;
class Material;


 /// This is just a emty core with no functionality at all

class PureCore : public Core
{

	public:

		 PureCore ();
		~PureCore ();


		/* virtual */ void enterCore (const Action* action);
		/* virtual */ void exitCore  (const Action* action);



  private:
    ///s
};


} // namespace graph
} // namespace gloost

#endif // GLOOST_PURECORE_H
