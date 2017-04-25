
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



// gloost system includes
#include <gloost/graph/Action.h>
#include <gloost/graph/PureCore.h>



namespace gloost
{
namespace graph
{



////////////////////////////////////////////////////////////////////////////////


 /// class constructor

PureCore::PureCore():
  Core(GLOOST_CORE_PURE)
{

}


////////////////////////////////////////////////////////////////////////////////


 /// class destructor

PureCore::~PureCore()
{

}


////////////////////////////////////////////////////////////////////////////////


 ///

/* virtual */
void
PureCore::enterCore (const Action* action)
{
  /// nothing
}


////////////////////////////////////////////////////////////////////////////////


 ///

/* virtual */
void
PureCore::exitCore (const Action* action)
{
	/// still nothing ... go away
}


////////////////////////////////////////////////////////////////////////////////



} // namespace graph
} // namespace gloost


