
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


#ifndef H_GLOOST_EMPTYCLASS
#define H_GLOOST_EMPTYCLASS

// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <memory>

namespace gloost
{
namespace subspace
{

  //  EmptyClass template

class EmptyClass
{
	public:

    /// a std::shared_ptr of an EmptyClass instance
    typedef std::shared_ptr<EmptyClass>       shared_ptr;
    typedef std::shared_ptr<const EmptyClass> const_shared_ptr;

    // class factory
    static shared_ptr create();

    // class destructor
	  virtual ~EmptyClass();

    // inits the EmptyClass
	  void init();


	protected:

    // class constructor
    EmptyClass();


	private:

   // ...

};

} // namespace subspace
} // namespace gloost


#endif // H_GLOOST_EMPTYCLASS


