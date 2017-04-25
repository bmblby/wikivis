
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



#ifndef H_GLOOST_MESSAGE_MESSAGE
#define H_GLOOST_MESSAGE_MESSAGE



// gloost includes
#include <gloost/BinaryBundle.h>


// cpp includes
#include <string>
#include <memory>
#include <memory>




namespace gloost
{
namespace postal
{


  //  Message

class Message : public std::enable_shared_from_this<Message>
{
	public:

    /// a std::shared_ptr of an Message instance
	  typedef std::shared_ptr<Message>       shared_ptr;


    // creates a Message instance
    static shared_ptr create(gloost::BinaryBundle::const_shared_ptr data = nullptr);


    // class destructor
	  virtual ~Message();

	  // returns the data of the message
	  gloost::BinaryBundle::const_shared_ptr getBundle();



	protected:

    // class constructor
    Message(gloost::BinaryBundle::const_shared_ptr data);



	private:

	  gloost::BinaryBundle::const_shared_ptr _data;
};


} // namespace postal

} // namespace gloost


#endif // H_GLOOST_MESSAGE_MESSAGE












