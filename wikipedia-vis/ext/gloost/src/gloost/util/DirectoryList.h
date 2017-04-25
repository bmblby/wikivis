
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



#ifndef H_GLOOST_UTIL_DIRECTORYLIST
#define H_GLOOST_UTIL_DIRECTORYLIST

// gloost
#include <gloost/gloostConfig.h>

// cpp includes
#include <string>
#include <vector>



namespace gloost
{
namespace util
{

  /// Opens a directory and provides a Container of file pathes, is able to filterpathes by substrings and extensions

class DirectoryList
{

public:

  // class constructor
//  DirectoryList(const std::string& path);
  DirectoryList(const gloost::PathType& path);
  // class destructor
  virtual ~DirectoryList();

  // container o file pathes
  typedef std::vector<gloost::PathType> FilePathContainer;


  // opens the directory, returns true if successful, filters by a given file extension
  bool open();
  bool openRecursive(const std::string& fileExtensionString = "");

  // removes all file entries that don't include the given substring
  virtual void filterbyFileSubstring(const std::string& filterString);

  virtual void filterByFileExtension(const std::string& fileExtensionString);
  virtual void filterByFileExtensionRecursive(const std::string& fileExtensionString);


  // returns a vector of strings with the entries of the directory
  virtual FilePathContainer&       getEntries();
  virtual const FilePathContainer& getEntries() const;

  // returns a vector of strings with the entries of the directory
  virtual std::vector<std::string>        getStringEntries() const;

private:

  FilePathContainer     _entries;
  gloost::PathType       _path;
};



/// ostream operator
extern std::ostream& operator<< (std::ostream&, const DirectoryList&);
extern std::wostream& operator<< (std::wostream&, const DirectoryList&);

} // namespace gloost
} // namespace util



#endif // #ifndef H_GLOOST_UTIL_DIRECTORYLIST

