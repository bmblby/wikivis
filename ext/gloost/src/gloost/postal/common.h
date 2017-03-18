
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



#ifndef H_GLOOST_POSTAL_COMMON_MESSAGE_TYPES
#define H_GLOOST_POSTAL_COMMON_MESSAGE_TYPES



#include <gloost/postal/Hub.h>
#include <gloost/postal/Message.h>
#include <gloost/gloostSerial.h>
#include <gloost/UniformTypes.h>
#include <gloost/postal/common.h>


namespace gloost
{

namespace postal
{

namespace common
{


/**
  \class   StringMessage

  \brief   creates, sends and decodes gloost::Postal::Message instances containing a string

  \author  Felix Weiszig
  \date    September 2013
  \remarks ...
*/


class StringMessage
{

  StringMessage() = delete;


public:

  /**
    \brief sends a message to a path
  */
  static void send(const std::string& text,
                   const std::string& path,
                   bool  recursive =  true)
  {
    BinaryBundle::shared_ptr idBundle      = gloost::serial::serialize((unsigned)70000001u);
    BinaryBundle::shared_ptr textBundle    = gloost::serial::stringBundle(text);
    BinaryBundle::shared_ptr messageBundle = idBundle + textBundle;

    gloost::postal::Hub::get()->send(path,
                                     Message::create(messageBundle),
                                     recursive);
  }


  /**
    \brief decodes a StringMessage after type check
  */

  static std::string decode(Message::shared_ptr message)
  {
    auto reader = message->getBundle()->createReader();
    std::string text("");

    unsigned id = reader->readUInt32();

    if (id == 70000001u)
    {
      text = gloost::serial::stringUnbundle(*reader);
    }

    return text;
  }

};




/**
  \class   ColoredStringMessage

  \brief   creates, sends and decodes gloost::Postal::Message instances containing a vec4 and a string

  \author  Felix Weiszig
  \date    September 2013
  \remarks ...
*/


class ColoredStringMessage
{

  ColoredStringMessage() = delete;


public:



  struct ColorAndText
  {
    ColorAndText():_color(1.0f,0.0f,0.0f,1.0f), _text(""){}
    vec4        _color;
    std::string _text;
  };



  /**
    \brief sends a message to a path
  */
  static void send(const std::string& text,
                   const vec4& color,
                   const std::string& path,
                   bool  recursive =  false)
  {
    BinaryBundle::shared_ptr idBundle      = gloost::serial::serialize((unsigned)70000002u);
    BinaryBundle::shared_ptr colorBundle   = gloost::serial::serialize(color);
    BinaryBundle::shared_ptr textBundle    = gloost::serial::stringBundle(text);
    BinaryBundle::shared_ptr messageBundle = BinaryBundle::create({idBundle, colorBundle, textBundle});

    gloost::postal::Hub::get()->send(path,
                                     Message::create(messageBundle),
                                     recursive);
  }

  /**
    \brief sends a message to a path
  */
  static void send(const std::string& text,
                   const std::string& path,
                   bool  recursive =  false)
  {
    send(text, vec4(1.0f, 1.0f, 1.0f, 1.0f), path, recursive);
  }


  /**
    \brief decodes a ColoredStringMessage after type check
  */

  static ColorAndText decode(Message::shared_ptr message)
  {
    auto reader = message->getBundle()->createReader();
    ColorAndText messageData;

    unsigned id = reader->readUInt32();

    if (id == 70000002u)
    {
      gloost::serial::unserialize(messageData._color, *reader);
      messageData._text  = gloost::serial::stringUnbundle(*reader);
    }

    return messageData;
  }

};


}  // namespace common
}  // namespace postal
}  // namespace gloost





#endif // H_GLOOST_POSTAL_COMMON_MESSAGE_TYPES
