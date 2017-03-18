
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
#include <gloost/util/Path.h>
#include <gloost/gloostHelper.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace util
{


/**
  \class   Path

  \brief   A class template because I'm to lazy to write a class from scratch

  \author  Felix Weiszig
  \date    May 2011
  \remarks Lazy mans way to create a new class ;-)
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Path::Path():
  _pathString("/")
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
Path::shared_ptr
Path::create(const std::string& pathString)
{
  auto instance = shared_ptr(new Path());
  instance->set(pathString);
	return instance;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

/*static*/
Path::shared_ptr
Path::create(const shared_ptr& path)
{
  auto instance = shared_ptr(new Path());
  instance->set(path);
	return instance;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Path::~Path()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the path
  \param   ...
  \remarks ...
*/

void
Path::set(const std::string& pathString)
{
	_pathString = pathString;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the path
  \param   ...
  \remarks ...
*/

void
Path::set(const shared_ptr&  path)
{
  _pathString = path->_pathString;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the path as string
  \param   ...
  \remarks ...
*/

const std::string&
Path::getAsString() const
{
  return _pathString;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the path as vector of the path components
  \param   ...
  \remarks ...
*/

std::vector<std::string>
Path::getAsVector() const
{
  return gloost::helper::stringExplode(_pathString, "/");;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns true if the path is well formed
  \param   ...
  \remarks ...
*/

bool
Path::isValid() const
{
  if (!_pathString.length())
  {
    return false;
  }
  if (*_pathString.c_str() != '/' )
  {
    return false;
  }
  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief    returns the depth of the path
  \param   ...
  \remarks ...
*/


size_t
Path::getDepth() const
{
  auto vector = getAsVector();
  return vector.size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief  returns true if the path contains a given subpath
  \param   ...
  \remarks ...
*/

bool
Path::containsSubpath(const std::string& subpath) const
{
  auto pos = _pathString.find(subpath);
	if(pos == std::string::npos)
  {
    return false;
  }

  auto subPathSlash = subpath + "/";

  if(subpath != "/" && !isLeaf(subpath) && _pathString.find(subPathSlash) == std::string::npos)
  {
    return false;
  }



  return true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Inits the Path
  \param   ...
  \remarks ...
*/

void
Path::init()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a string with the parent folders path
  \param   ...
  \remarks ...
*/

Path::shared_ptr
Path::withoutLastElement()
{
  auto found = _pathString.rfind("/");
  if(found == std::string::npos)
  {
    return Path::create(_pathString);
  }

  auto path = _pathString.substr(0, found);
  return Path::create(path);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a string with element after the subpath
  \param   ...
  \remarks ...
*/

std::string
Path::elementAfterSubpath(const std::string& subpath) const

{
auto substring = _pathString.substr(0, subpath.length());
  if(substring != subpath)
  {
    return "";
  }

  auto length = _pathString.find("/",subpath.length());
  if(length == std::string::npos)
  {
    return "";
  }

  return _pathString.substr(subpath.length(), length);



}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a string with the subpath following a given subpath
  \param   ...
  \remarks ...
*/

Path::shared_ptr
Path::pathAfterSubpath(const std::string& subpath) const
{
  auto substring = _pathString.substr(0, subpath.length());
  if(substring != subpath)
  {
    return nullptr;
  }

  // search for the next "/" in the path
  auto pos = _pathString.find("/",subpath.length()+1);
  if(pos == std::string::npos)
  {
    return Path::create(_pathString);
  }

  // create the new subpath
  auto newSubpath = _pathString.substr(0, pos);

  return Path::create(newSubpath);



}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a string with the parent folders path
  \param   ...
  \remarks ...
*/

std::string
Path::elementBeforeSubpath(const std::string& path, const std::string& subpath)
{
  auto pos = path.find(subpath);

  if(pos == std::string::npos)
  {
    return "";
  }

  auto subString = path.substr(0,pos);
//  subString =  removeLastElement(subString);


  return (subString);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a string with the parent folders path
  \param   ...
  \remarks ...
*/

/*static*/
std::string
Path::lastElement() const
{
  auto pos = _pathString.rfind("/");
  return _pathString.substr(pos+1, _pathString.length() - pos);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a string with the parent folders path
  \param   ...
  \remarks ...
*/

/*static*/
bool
Path::isLeaf(const std::string& element) const
{
  auto pos = _pathString.find(element);

  if(pos == std::string::npos)
  {
    std::cerr << std::endl << "D'OH from Path::isLeaf():     ";
    std::cerr << std::endl << "          Element not in Path.";
    return false;
  }

  pos = _pathString.find("/", pos);
  if(pos == std::string::npos)
  {
    return true;
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////


/* extern */
std::ostream&
operator<< (std::ostream& os, const Path& path)
{
  os << path.getAsString();
  return os;
}


////////////////////////////////////////////////////////////////////////////////


  ///

/* extern */
std::ostream&
operator<< (std::ostream& os, const Path::shared_ptr& path)
{
  os << path->getAsString();
  return os;
}





} // namespace util
} // namespace gloost


