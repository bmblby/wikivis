
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


#ifndef GLOOST_TEXTLOG_H
#define GLOOST_TEXTLOG_H



// gloost system includes
#include<gloost/TextureManager.h>
#include<gloost/obsolete/TextureText.h>
#include<gloost/UniformTypes.h>


// cpp includes
#include <string>
#include <vector>



namespace gloost
{

 ///  single TextLog text line with textline and color

struct TextLogMessage
{
  TextLogMessage()
  {
    text = std::string();
    color   = vec3(1.0f, 1.0f, 1.0f);
  }

  TextLogMessage(const std::string& textline)
  {
    text = textline;
    color   = vec3(1.0f, 1.0f, 1.0f);
  }

  TextLogMessage(const std::string& textline, vec3 textcolor)
  {
    text  = textline;
    color = textcolor;
  }

  std::string text;
  vec3        color;
};


///////////////////////////////////////////////////////////////////////////////


  /// stream operator for a TextLog message

extern std::ostream& operator<< (std::ostream& os, const TextLogMessage& TLM);


///////////////////////////////////////////////////////////////////////////////


  ///  provides a renderable text log

class TextLog
{
	public:

    /// get an instance to the Textlog
    static TextLog* getInstance(const std::string& fontTexturePath = "");

    /// class destructor
	  ~TextLog();


	  /// add a line of text with color
	  void addLine(const std::string& textLine = " ",
                 const vec3& color = vec3(1.0, 1.0, 1.0));


	  /// add a line of text as message
	  void addLine(const TextLogMessage& message);


    /// clear log
    void clear();

    /// write log
    void writeLog (std::string filename);


	  /// scroll up
    void scrollUp(unsigned int lines = 1);

    /// scroll down
    void scrollDown(unsigned int lines = 1);


    /// render the log to the screen
    void draw();


    /// get a pointer to the gloost::TextureText object to configure it
    TextureText* getTextureTextObject();



    /// get Position of the log
    const vec2& getPosition();

    /// set Position of the log
    void setPosition(const vec2& position);


    /// set number of lines to view
    void setLinesToView(unsigned int number);



	private:

    ///
    static TextLog* _theinstance;


    /// texture manager
    gloost::TextureManager* _texManager;

    /// texture text
    gloost::TextureText* _textMaker;


    /// position of the loc (position of the upper left corner)
    vec2 _position;

    /// vector of messages
    std::vector<TextLogMessage> _messages;

    /// how many line to show
    unsigned int _linesToShow;

    /// scroll position
    int _scrollPos;


	  /// class constructor
    TextLog(const std::string& fontTexturePath);



};


} // namespace gloost


#endif // GLOOST_TEXTLOG_H


