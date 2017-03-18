
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
#include <gloost/FontCache.h>
#include <gloost/FreeTypeWriter.h>



// cpp includes
#include <string>
#include <iostream>





namespace gloost
{

/**
  \class   FontCache

  \brief   Stores instances of FreeTypeWriters to provide application width access

  \author  Felix Weiszig
  \date    October 2011
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/*static*/ FontCache* FontCache::_instance = 0;


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the instance of the font cache
  \remarks ...
*/

/*static*/
FontCache*
FontCache::get()
{
  if (!_instance)
  {
    _instance = new FontCache();
  }
  return _instance;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

FontCache::FontCache():
    _cache(),
    _idCounter(0)
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

FontCache::~FontCache()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a FreeTypeWriter and returns a id
  \remarks ...
*/

unsigned
FontCache::create(const std::string& fontFilePath, unsigned size, bool smooth)
{
  FreeTypeWriter* writer = new FreeTypeWriter(fontFilePath, size, smooth);
  _cache[_idCounter] = writer;

  unsigned tmp = _idCounter;
  ++_idCounter;
  return tmp;

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   adds a FreeTypeWriter and returns a id
  \remarks ...
*/

unsigned
FontCache::add(FreeTypeWriter* writer)
{
  _cache[_idCounter] = writer;

  unsigned tmp = _idCounter;
  ++_idCounter;
  return tmp;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a FreeTypeWriter for an id
  \remarks ...
*/

FreeTypeWriter*
FontCache::getById(unsigned id)
{
  std::map<unsigned, FreeTypeWriter*>::iterator pos = _cache.find(id);

  if (pos != _cache.end())
  {
    return (*pos).second;
  }

  std::cerr << std::endl;
  std::cerr << std::endl << "ERROR in FontCache::getById(unsigned id): ";
  std::cerr << std::endl << "         Could not find FreeTypeWriter with id " << id;
  std::cerr << std::endl;

  return 0;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


