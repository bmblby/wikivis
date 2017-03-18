
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
#include <gloost/obsolete/TextLog.h>
#include <gloost/gloostMath.h>
#include <gloost/obsolete/gl/gloostRenderGoodies.h>


// cpp includes
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>


namespace gloost
{


/*static*/
TextLog* TextLog::_theinstance = 0;


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

TextLog::TextLog(const std::string& fontTexturePath):
  _texManager(gloost::TextureManager::getInstance()),
  _textMaker(new gloost::TextureText(fontTexturePath)),
  _position(100,100),
  _messages(),
  _linesToShow(15),
  _scrollPos(0)
{
	clear();

	_textMaker->setCharSpace(-(_textMaker->getCharWidth()/3.0));
}


///////////////////////////////////////////////////////////////////////////////


  /// get an instance to the Textlog

TextLog*
TextLog::getInstance(const std::string& fontTexturePath)
{
  if(!_theinstance)
  {
    if (!std::strcmp("", fontTexturePath.c_str()))
    {
      std::cout << std::endl;
      std::cout << std::endl << "ERROR from TextLog::getInstance(std::string):" ;
      std::cout << std::endl << "            No font texture given. Please specify a font texture when you";
      std::cout << std::endl << "            call TextLog::getInstance(std::string fontTexturePath) the first";
      std::cout << std::endl << "            time. A large collection of font textures can be found in";
      std::cout << std::endl << "            \"<gloost>/data/fonts\"";
      std::cout << std::endl << "            A good choice for the TextLog is gloost_Monaco_12.png";
      std::flush(std::cout);
    }



    _theinstance = new TextLog(fontTexturePath);
  }

  return _theinstance;
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

TextLog::~TextLog()
{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


  /// add a line of text

void
TextLog::addLine(const std::string& textLine,
	               const vec3& color)
{
  addLine(TextLogMessage(textLine, color));
}


///////////////////////////////////////////////////////////////////////////////


  /// add a line of text as message

void
TextLog::addLine(const TextLogMessage& message)
{

  // scroll only if we are scrolled to the end of all messages
  if ((unsigned int)_scrollPos == _messages.size())
  {
    ++_scrollPos;
  }

  _messages.push_back(message);
}


///////////////////////////////////////////////////////////////////////////////


  /// clear log

void
TextLog::clear()
{
  _messages.clear();

  _scrollPos = 0;

  addLine("gloost::TextLog");
  addLine();
}


///////////////////////////////////////////////////////////////////////////////


  /// write log

void
TextLog::writeLog (std::string filename)
{

  addLine();
  addLine("Write this log to file " + filename);


  /// write txt file
  std::ofstream txtFile;
  txtFile.open(filename.c_str());

  txtFile << "gloost::TextLog with " << _messages.size() << " entries: " << std::endl << std::endl;


  for(unsigned int i=0; i != _messages.size(); ++i)
  {
    txtFile << i << ": " << _messages[i] << std::endl;
  }

  std::flush(txtFile);
  txtFile.close();


  /// write html file
  std::ofstream htmlFile;
  htmlFile.open((filename+".html").c_str());



  htmlFile << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << std::endl;
  htmlFile << "<html>" << std::endl;
  htmlFile << "  <head>" << std::endl;
  htmlFile << "    <meta http-equiv=\"content-type\" content=\"text/html; charset=windows-1250\">" << std::endl;
  htmlFile << "    <meta name=\"generator\" content=\"PSPad editor, www.pspad.com\">" << std::endl;
  htmlFile << "    <title>gloost::TextLog html super power</title>" << std::endl;
  htmlFile << "  </head>" << std::endl;
  htmlFile << "  <body>" << std::endl;
  htmlFile << "     <table>" << std::endl;

  for(unsigned int i=0; i != _messages.size(); ++i)
  {
    htmlFile << "       <tr>" << std::endl;
    htmlFile << "         <td width=\"25\">" << std::endl;
    htmlFile << "           " << (char)(i+32) << ".";
    htmlFile << "         </td>" << std::endl;
    htmlFile << "         <td>" << std::endl;
    htmlFile << "           " << _messages[i] << std::endl;
    htmlFile << "         </td>" << std::endl;
    htmlFile << "       </tr>" << std::endl;
  }

  htmlFile << "     </table>" << std::endl;
  htmlFile << "  <body>" << std::endl;

  htmlFile << "  </body>" << std::endl;
  htmlFile << "</html>" << std::endl;

  std::flush(htmlFile);
  htmlFile.close();

}


///////////////////////////////////////////////////////////////////////////////


  /// scroll up

void
TextLog::scrollUp(unsigned int lines)
{
  if ((unsigned int)_scrollPos > _linesToShow)
  {
    _scrollPos -= lines;
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// scroll down

void
TextLog::scrollDown(unsigned int lines)
{
  _scrollPos += lines;

  if ((unsigned int ) _scrollPos > _messages.size()-1)
  {
    _scrollPos = _messages.size();
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// render the log to the screen

void
TextLog::draw()
{
  /// write the text from startPos to
  int startPos = math::max(int(_scrollPos - _linesToShow), (int) 0);


  _textMaker->begin();
  _textMaker->setWritePosition(_position);

  for(int i=startPos; i!=_scrollPos; ++i)
  {
    glColor4f(_messages[i].color.x, _messages[i].color.y, _messages[i].color.z, 1.0);
    _textMaker->renderTextLine(gloost::helper::toString(i) + ": " + _messages[i].text);
  }


  if ((unsigned int)_scrollPos != _messages.size())
  {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    _textMaker->renderTextLine(std::string("(+") +  gloost::helper::toString(_messages.size()-_scrollPos) + std::string(") ") );
  }

  _textMaker->end();

}


///////////////////////////////////////////////////////////////////////////////


  /// get a pointer to the gloost::TextureText object to configure it

TextureText*
TextLog::getTextureTextObject()
{
  return _textMaker;
}


///////////////////////////////////////////////////////////////////////////////


  /// get Position of the log

const vec2&
TextLog::getPosition()
{
  return _position;
}


///////////////////////////////////////////////////////////////////////////////


  /// set Position of the log

void
TextLog::setPosition(const vec2& position)
{
  _position = position;
}


///////////////////////////////////////////////////////////////////////////////


  /// set number of lines to view

void
TextLog::setLinesToView(unsigned int number)
{
  _linesToShow = number;
}


///////////////////////////////////////////////////////////////////////////////


  /// stream operator for a TextLog message

/*extern*/
extern std::ostream& operator<< (std::ostream& os, const TextLogMessage& TLM)
{
  os << TLM.text;
  return os;
}


} // namespace gloost


