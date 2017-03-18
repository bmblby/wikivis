
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



#ifndef H_GLOOST_MESSAGE_SUBSCRIBER
#define H_GLOOST_MESSAGE_SUBSCRIBER


// gloost includes
#include <gloost/postal/Message.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{
namespace postal
{


  //  Subscriber

class Subscriber : public std::enable_shared_from_this<Subscriber>
{
	public:

    /// a std::shared_ptr of an Subscriber instance
	  typedef std::shared_ptr<Subscriber>       shared_ptr;
    typedef std::shared_ptr<const Subscriber> const_shared_ptr;


    // creates a Subscriber instance
    static shared_ptr create();
    static shared_ptr create(const std::string& name);



    // connects this node to a path
    bool connect(const std::string& path);

    // disconnect this node from a path
    bool disconnect(const std::string& path);


    // class destructor
	  virtual ~Subscriber();



	  // returns the name of the Subscriber
	  virtual const std::string& getName() const;






	  // receive a message
	  virtual bool receive(const std::string& path, Message::shared_ptr message);



	protected:

    // class constructor
    Subscriber();

    Subscriber(const std::string& name);



	private:

	  std::string _name;


};


} // namespace postal

} // namespace gloost


#endif // H_GLOOST_MESSAGE_SUBSCRIBER












