
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



#ifndef H_GLOOST_MESSAGEWRAPPER
#define H_GLOOST_MESSAGEWRAPPER



// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{
namespace postal
{

  class Message;


  //  base class of all MessageWrappers, inheritor performing serialization and unserialization of Messages

class MessageWrapper
{
  public:

    // class cosntructor
    MessageWrapper(unsigned typeId);
    MessageWrapper(std::shared_ptr<gloost::postal::Message> message);

    // class destructor
    ~MessageWrapper();


    // returns the message
    std::shared_ptr<gloost::postal::Message> getMessage();


  protected:

    // serializes messages fields to a gloost::BinaryBundle
    virtual bool toMessage() = 0;

    // unserializes messages fields from a gloost::BinaryBundle
    virtual bool fromMessage() = 0;


  protected:
    unsigned                                 _typeId  = 0;
    std::shared_ptr<gloost::postal::Message> _message = nullptr;

};


} // namespace postal
} // namespace gloost


#endif // H_GLOOST_MESSAGEWRAPPER


