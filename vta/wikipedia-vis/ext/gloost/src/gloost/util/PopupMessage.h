
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

            gloost is being created by Felix Weißig and Stephan Beck

     Felix Weißig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/

#ifndef H_LIBGLOOST_UTIL_POPUPMESSAGE
#define H_LIBGLOOST_UTIL_POPUPMESSAGE



// gloost includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{
namespace util
{


  //  PopupMessage

class PopupMessage
{
	public:

    // sends the PopupMessage
	  static void sendMessage(const std::string& header,
                            const std::string& message,
                            unsigned fadeOutMilli = 0);

	protected:


	private:

    // class constructor
    PopupMessage();
   // ...

};


} // namespace util
} // namespace gloost


#endif //H_LIBGLOOST_UTIL_POPUPMESSAGE
