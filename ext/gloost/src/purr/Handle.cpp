
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

// purr system includes
#include <purr/Handle.h>

// cpp includes
#include <string>
#include <iostream>

namespace purr
{

/**
  \class   Handle
  \brief   Stores a handle as string but supports fast comparison via a hash
  \author  Felix Weiszig
  \date    Feb 2016
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////

/*static*/ std::atomic<unsigned> Handle::_hashCounter(0u);
/*static*/ std::mutex Handle::_nameToHashMapMutex;
/*static*/ std::map<std::string, unsigned> Handle::_nameToHashMap = std::map<std::string, unsigned>();
///*static*/ std::map<unsigned, std::string> Handle::_hashToName = std::map<unsigned, std::string>();

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

Handle::Handle(const std::string& name):
  _hash(toHash(name))
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Handle::~Handle()
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Inits the Handle
  \param   ...
  \remarks ...
*/

/*static*/
unsigned
Handle::toHash(const std::string& name)
{
  std::lock_guard<std::mutex> lock(_nameToHashMapMutex);
	const auto pos = _nameToHashMap.find(name);
	if (pos != _nameToHashMap.end())
  {
    return pos->second;
  }

  const auto newHash = _hashCounter++;
  _nameToHashMap[name] = newHash;
  return newHash;
}

////////////////////////////////////////////////////////////////////////////////

bool
Handle::operator==(const Handle& handle)
{
  return _hash == handle._hash;
}

////////////////////////////////////////////////////////////////////////////////

bool
Handle::operator!=(const Handle& handle)
{
  return _hash != handle._hash;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace purr
