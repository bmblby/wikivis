
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



#ifndef H_GLOOST_FONTCACHE
#define H_GLOOST_FONTCACHE



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <map>



namespace gloost
{

  class FreeTypeWriter;



  // Stores instances of FreeTypeWriters to provide application width access

class FontCache
{
	public:

    // returns the instance of the font cache
    static FontCache* get();

    // class destructor
	  ~FontCache();


    // creates a FreeTypeWriter and returns a id
	  unsigned create(const std::string& fontFilePath, unsigned size, bool smooth);

    // adds a FreeTypeWriter and returns a id
	  unsigned add(FreeTypeWriter* writer);

	  // returns a FreeTypeWriter for a id
	  FreeTypeWriter* getById(unsigned id);


	private:

    // class constructor
    FontCache();

    static FontCache* _instance;

    std::map<unsigned, FreeTypeWriter*> _cache;

    unsigned _idCounter;
};


} // namespace gloost


#endif // H_GLOOST_FONTCACHE


