

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
#include <gloost/log/observer/ToXmlFile.h>
#include <gloost/log/Log.h>
#include <gloost/log/Message.h>
#include <gloost/gloostHelper.h>
#include <gloost/BinaryFile.h>



// cpp includes
#include <string>
#include <iostream>
#include <fstream>



namespace gloost
{
namespace log
{
namespace observer
{


/**
  \class   ToXmlFile

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

ToXmlFile::ToXmlFile(const gloost::PathType& filePath):
  ObserverBase(),
  _filePath(filePath)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   factory
  \remarks ...
*/

/*static*/
ToXmlFile::shared_ptr
ToXmlFile::create(const gloost::PathType& filePath)
{
	return shared_ptr(new ToXmlFile(filePath));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
ToXmlFile::~ToXmlFile()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ToXmlFile::onBegin (const std::shared_ptr<Log>& log)
{
  auto guard = log->lockMessages();
//
//  auto newDoc = std::shared_ptr<tinyxml2::XMLDocument>(new tinyxml2::XMLDocument());
//  tinyxml2::XMLDeclaration* decl = newDoc->NewDeclaration("xml version=\"1.0\" standalone=\"yes\"");
//
//  newDoc->InsertEndChild(decl);

//  here


  auto outfile = gloost::BinaryFile::create();
  outfile->openToWrite(_filePath);
//  if (outfile.is_open())
  {
    outfile->writeString("<?xml version=\"1.0\" standalone=\"yes\"?>");
    for (const auto& message : log->getMessages())
    {
      appendAsXmlElement(outfile, message);
    }
    outfile->close();
  }
}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ToXmlFile::onPostMessage (const std::shared_ptr<const Message>& message)
{
  auto outfile = gloost::BinaryFile::create();
  outfile->openToAppend(_filePath);
//  if (outfile.is_open())
  {
    appendAsXmlElement(outfile, message);
    outfile->close();
  }

}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ToXmlFile::onEnd (const std::shared_ptr<Log>&)
{

}

////////////////////////////////////////////////////////////////////////////////

/*virtual*/
void
ToXmlFile::appendAsXmlElement(std::shared_ptr<gloost::BinaryFile> outFile,
                              const std::shared_ptr<const Message>& message)
{
  std::stringstream stream;
  stream << std::endl << "<log_entry>";
  stream << std::endl << "  <location value=\"" << message->getLocationString() << "\"/>";
  stream << std::endl << "  <level    value=\"" << message->getLogLevel() << "\"/>";
  stream << std::endl << "  <time     value=\"" << message->getTime() << "\"/>";
  stream << std::endl << "  <text     value=\"" << gloost::helper::stringSearchAndReplace(message->asString(), "\"", "'") << "\"/>";
  stream << std::endl << "</log_entry>";
  outFile->writeString(stream.str());
}

////////////////////////////////////////////////////////////////////////////////





} // namespace observer
} // namespace log
} // namespace gloost


