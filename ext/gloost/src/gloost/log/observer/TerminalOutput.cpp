
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
#include <gloost/log/observer/TerminalOutput.h>
#include <gloost/log/Log.h>
#include <gloost/log/Message.h>
#include <gloost/util/PopupMessage.h>
#include <gloost/gloostHelper.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace log
{
namespace observer
{


/**
  \class   TerminalOutput

  \brief   Base of all Log Observer

  \author  Felix Weiszig, Dynardo GmbH
  \date    Feb 2015
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

TerminalOutput::TerminalOutput():
  ObserverBase(),
  _messageCounter(0u),
  _terminalMutex()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   factory
  \remarks ...
*/

/*static*/
TerminalOutput::shared_ptr
TerminalOutput::create()
{
	return shared_ptr(new TerminalOutput());
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
TerminalOutput::~TerminalOutput()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
TerminalOutput::onBegin (const std::shared_ptr<Log>& log)
{
  auto guard = log->lockMessages();
  for (const auto& message : log->getMessages())
  {
    onPostMessage(message);
  }
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
TerminalOutput::onPostMessage (const std::shared_ptr<const Message>& message)
{
  std::lock_guard<std::mutex> lock(_terminalMutex);
  std::cerr << std::endl << "(" << _messageCounter << ") ---";
  std::cerr << message->asFormatedString();
  std::cerr << std::endl << "(/" << _messageCounter << ") ---";
  std::cerr << std::endl;
  ++_messageCounter;

  if (message->getLogLevel() == LogLevel::POPUP)
  {
    gloost::util::PopupMessage::sendMessage(std::string("Tamino says: "),
                                            message->asString(),
                                            6000);
  }
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
TerminalOutput::onEnd (const std::shared_ptr<Log>&)
{

}

////////////////////////////////////////////////////////////////////////////////


} // namespace observer
} // namespace log
} // namespace gloost


