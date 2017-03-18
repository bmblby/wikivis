
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



#ifndef H_GLOOST_UTIL_PATH
#define H_GLOOST_UTIL_PATH

// cpp includes
#include <string>
#include <memory>
#include <vector>
#include <unordered_set>

namespace gloost
{
namespace util
{
  //  A hierachical path with utility functions

class Path
{
	public:

    /// a std::shared_ptr of an Path instance
    typedef std::shared_ptr<Path>       shared_ptr;
    typedef std::shared_ptr<const Path> const_shared_ptr;

    // class factoy
    static shared_ptr create(const std::string& pathString = "/");
    // class factoy
    static shared_ptr create(const shared_ptr& path);

    struct SharedPtrEqualTo
    {
      bool operator() (const shared_ptr& pathA, const shared_ptr& pathB) const
      {
        if (pathA->getAsString() == pathB->getAsString())
        {
          return true;
        }
        return false;
      }
    };

    struct SharedPtrPathHash
    {
      std::size_t operator()(const shared_ptr& s) const
      {
          std::size_t h1 = std::hash<std::string>()(s->getAsString());
          return h1;
      }
    };

    typedef std::unordered_set<std::shared_ptr<Path>, SharedPtrPathHash, SharedPtrEqualTo> PathSet;

    // sets the path
    void set(const std::string& pathString);
    void set(const shared_ptr&  path);

    // returns the path as string
    const std::string& getAsString() const;

    // returns the path as vector of the path components
    std::vector<std::string> getAsVector() const;

    // returns true if the path is well formed
    bool isValid() const;

    // returns the depth of the path
    size_t getDepth() const;

    // returns true if the path contains a given subpath
    bool containsSubpath(const std::string& subpath) const;

    // returns a string with the parent folders path
    shared_ptr withoutLastElement();

    // returns a string with the element following a given subpath
    std::string elementAfterSubpath(const std::string& subPath) const;


    // returns a string with the subpath follwing a given subpath
    shared_ptr pathAfterSubpath(const std::string& subPath) const;

    // returns a string with the subpath follwing a given subpath
    std::string pathEndingWithSubpath(const std::string& subPath) const;

    // returns a string with the parent folders path
    static std::string elementBeforeSubpath(const std::string& path, const std::string& subPath);

    // returns a string with the parent folders path
    std::string lastElement() const;

    // returns a string with the parent folders path
    bool isLeaf(const std::string& element) const;


    // class destructor
	  virtual ~Path();

    // inits the Path
	  void init();

	protected:

    // class constructor
    Path();


	private:

    std::string _pathString;
};

// ostream operator
extern std::ostream& operator<< (std::ostream&, const Path&);
extern std::ostream& operator<< (std::ostream&, const Path::shared_ptr&);

} // namespace util
} // namespace gloost

#endif // H_GLOOST_UTIL_PATH
