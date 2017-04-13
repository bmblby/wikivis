
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


#ifndef H_GLOOST_GLOBALPROPERTIES
#define H_GLOOST_GLOBALPROPERTIES


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/PropertySet.h>


// cpp includes
#include <string>



namespace gloost
{


  //  A PropertySet as singelton for application wide variables and configurations

class GlobalProperties: public PropertySet
{
	public:

	  // returns an instance of this class
	  static GlobalProperties* get();

    // class destructor
	  virtual ~GlobalProperties();


//    GlobalProperties();
//    GlobalProperties(const PropertySet& propSet);
//    GlobalProperties(const std::string& xmlFilePath);


  protected:

    // class constructor
    GlobalProperties();

    static GlobalProperties* _theInstance;

	private:



};


} // namespace gloost


#endif // H_GLOOST_GLOBALPROPERTIES


