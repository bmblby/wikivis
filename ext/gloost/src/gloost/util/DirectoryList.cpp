
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


// gloost includes
#include <gloost/util/DirectoryList.h>
#include <gloost/gloostHelper.h>

// boost
#include <boost/filesystem.hpp>

// cpp includes
#include <stddef.h>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <algorithm>

namespace gloost
{
namespace util
{

/**
  \class   DirectoryList

  \brief   Opens a directory and provides a Container of file pathes, is able to filterpathes by substrings and extensions

  \author  Felix Weiszig
  \date    March 2013
  \remarks Lazy mans way to create a new class ;-)
*/

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class constructor
  \remarks ...
*/
//
//DirectoryList::DirectoryList(const std::string& path): DirectoryList(gloost::PathType(path))
//{}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class constructor
  \remarks ...
*/

DirectoryList::DirectoryList(const gloost::PathType& path):
  _entries(),
  _path(path)
{}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class destructor
  \remarks ...
*/

DirectoryList::~DirectoryList()
{}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens the directory, returns true if successful, filters by a given file extension
  \remarks ...
*/

/*virtual*/
bool
DirectoryList::open()
{
  _entries.clear();
  if(!boost::filesystem::is_directory(_path))
  {
//    gloost::log::GlobalLog::postDebug(GLOOST_CURRENT_LOCATION,
//                                      _path.string() + " is not a directory ");

    return false;
  }

  boost::filesystem::directory_iterator begin(_path);
  boost::filesystem::directory_iterator end;


  std::vector<boost::filesystem::directory_entry> directoryEntries(begin, end);

  for (const auto& directoryEntry : directoryEntries)
  {
    const auto& p = directoryEntry.path();
    _entries.emplace_back(p);
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens the directory, returns true if successful, filters by a given file extension
  \remarks ...
*/

/*virtual*/
bool
DirectoryList::openRecursive(const std::string& fileExtensionString)
{
  _entries.clear();
  boost::filesystem::recursive_directory_iterator begin(_path), end;
  std::vector<boost::filesystem::directory_entry> directoryEntries(begin, end);

  for (const auto& directoryEntry : directoryEntries)
  {
    const auto& p = directoryEntry.path();
//    if(boost::filesystem::is_directory(p))
//    {
      _entries.emplace_back(p);
//    }
  }

  if(!fileExtensionString.empty())
  {
    filterByFileExtension(fileExtensionString);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes all file entries that don't include the given substring
  \remarks ...
*/

/*virtual*/
void
DirectoryList::filterbyFileSubstring(const std::string& pattern)
{
  FilePathContainer survivors;
  for (unsigned i=0; i!=_entries.size(); ++i)
  {
    if ( helper::pathToFilename(_entries[i]).string().find(pattern) != std::string::npos)
    {
      survivors.push_back(_entries[i]);
    }
  }

  survivors.swap(_entries);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes all file entries that don't include the given substring
  \remarks ...
*/

/*virtual*/
void
DirectoryList::filterByFileExtension(const std::string& fileExtensionString)
{
  auto tempEntries = _entries;
  _entries.clear();
  for (const auto& directoryPath : tempEntries)
  {
    if(directoryPath.extension().string() == fileExtensionString)
    {
      _entries.emplace_back(directoryPath);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   removes all file entries that don't include the given substring
  \remarks ...
*/

/*virtual*/
void
DirectoryList::filterByFileExtensionRecursive(const std::string& fileExtensionString)
{
  auto tempEntries = _entries;
  _entries.clear();
  for (const auto& directoryPath : tempEntries)
  {
    boost::filesystem::recursive_directory_iterator begin(directoryPath), end;
    std::vector<boost::filesystem::directory_entry> directoryEntries(begin, end);

    for (const auto& directoryEntry : directoryEntries)
    {
      const auto& p = directoryEntry.path();
      if(p.extension() == boost::filesystem::path(fileExtensionString))
      {
        _entries.emplace_back(directoryPath);
        break;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of strings with the entries of the directory
  \remarks ...
*/

/*virtual*/
DirectoryList::FilePathContainer&
DirectoryList::getEntries()
{
  return _entries;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of strings with the entries of the directory
  \remarks ...
*/

/*virtual*/
const DirectoryList::FilePathContainer&
DirectoryList::getEntries() const
{
  return _entries;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of strings with the entries of the directory
  \remarks ...
*/

/*virtual*/
std::vector<std::string>
DirectoryList::getStringEntries() const
{
  std::vector<std::string> stringEntries(_entries.size());
  for (unsigned i=0u; i!=_entries.size(); ++i)
  {
    stringEntries[i] = _entries[i].string();
  }
  return stringEntries;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   out stream operator
  \remarks ...
*/

/* extern */
std::ostream&
operator<< (std::ostream& os, const DirectoryList& dl)
{
  DirectoryList::FilePathContainer entries = dl.getEntries();

  os << std::endl << "DirectoryList:";

  for (unsigned int i=0; i != entries.size(); ++i)
  {
    os << std::endl << "  " << entries[i].string();
  }

  return os;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   out stream operator
  \remarks ...
*/

/* extern */
std::wostream&
operator<< (std::wostream& os, const DirectoryList& dl)
{
  DirectoryList::FilePathContainer entries = dl.getEntries();

  os << std::endl << gloost::PathType("DirectoryList:").wstring();

  for (unsigned int i=0; i != entries.size(); ++i)
  {
    os << std::endl << "  " << entries[i].wstring();
  }

  return os;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost
} // namespace util
