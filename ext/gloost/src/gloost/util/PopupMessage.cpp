
// gloost includes
#include <gloost/util/PopupMessage.h>

// cpp includes
#include <string>
#include <iostream>

#ifdef GLOOST_WINDOWS
#include <windows.h>
#endif // GLOOST_WINDOWS

namespace gloost
{
namespace util
{

/**
  \class   PopupMessage

  \brief   creates a popup message; only works for system with notify_send installed or windows

  \author  Benjamin Bendig Rodriguez
  \date    September 2014
  \remarks Lazy mans way to create a new class ;-)
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

PopupMessage::PopupMessage()
{
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sends the PopupMessage
  \param   ...
  \remarks fadeOutMilli for linux only
*/

void
PopupMessage::sendMessage(const std::string& header, const std::string& message, unsigned fadeOutMilli)
{
  #ifdef GLOOST_GNULINUX
    std::string quotedHeader = "'" + header + "'";
    std::string quotedMessage = "'" + message + "'";
    std::string send = std::string("/usr/bin/notify-send -t ") + std::to_string(fadeOutMilli) + " " + quotedHeader + " " + quotedMessage;
    auto result = std::system(send.c_str());
  #elif GLOOST_WINDOWS
    MessageBox( 0, (LPCSTR)(message.c_str()), (LPCSTR)"gloost messages", 0 );
  #endif // GLOOST_WINDOWS
}

////////////////////////////////////////////////////////////////////////////////

} // namespace util
} // namespace fancy
