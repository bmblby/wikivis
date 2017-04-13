
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

             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/

#ifndef H_PURR_HANDLE
#define H_PURR_HANDLE

// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <map>
#include <memory>
#include <atomic>
#include <mutex>

namespace purr
{
  //  Stores a handle as string but supports fast comparison via a numeric hash

class Handle
{
	public:

    /// a std::shared_ptr of an Handle instance
    typedef std::shared_ptr<Handle>       shared_ptr;
    typedef std::shared_ptr<const Handle> const_shared_ptr;

    // class constructor
    Handle(const std::string& name);

    // class destructor
	  virtual ~Handle();


    bool operator==(const Handle& handle);
    bool operator!=(const Handle& handle);

	protected:

	private:

	  unsigned _hash;

	  // static stuff for all
	  static std::atomic<unsigned>           _hashCounter;
	  static std::mutex                      _nameToHashMapMutex;
    static std::map<std::string, unsigned> _nameToHashMap;
//    static std::map<unsigned, std::string> _hashToName;

    // returns a hash value for a string
    static unsigned toHash(const std::string& name);
};

} // namespace purr

#endif // H_PURR_HANDLE

