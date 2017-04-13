
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


#ifndef H_GLOOST_CMDPARSER_H
#define H_GLOOST_CMDPARSER_H

#include <string>
#include <vector>
#include <map>



namespace gloost
{


  ///  Usefull utility class to handle and document comandline parameters

class CmdParser
{
  public:
    CmdParser(const std::string& arguments = "");
    ~CmdParser();

    void addOpt(std::string opt, int numValues, std::string optlong, std::string help = "");
    void showHelp();
    void init(int& argc, char** argv);

    int isOptSet(std::string opt);

    std::vector<int>         getOptsInt(std::string);
    std::vector<float>       getOptsFloat(std::string);
    std::vector<std::string> getOptsString(std::string);


    std::vector<std::string> getArgs() const;

  private:
    std::map<std::string,std::vector<std::string>* > _opts;
    std::map<std::string,int>                        _optsNumValues;
    std::map<std::string,std::string>                _optslong;
    std::vector<std::string>                         _args;
    std::string                                      _help;
    std::string                                      _arguments;
};

} // namespace gloost


#endif //#ifndef H_GLOOST_CMDPARSER_H
