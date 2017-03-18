
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


#ifndef H_GLOOST_HANDLEMANAGER
#define H_GLOOST_HANDLEMANAGER

// gloost system includes
#include <gloost/gloostConfig.h>

// cpp includes
#include <string>
#include <memory>
#include <mutex>
#include <stack>

namespace gloost
{
  //  Provides system wide unique ids/handles

class HandleManager
{
	public:

    typedef size_t HandleType;

    /// a std::shared_ptr of an HandleManager instance
//    typedef std::shared_ptr<HandleManager>       shared_ptr;
//    typedef std::shared_ptr<const HandleManager> const_shared_ptr;

    // class factory
    static HandleManager* get();

    // class destructor
	  virtual ~HandleManager();

    // provides a unique handle
	  HandleType grabHandle();

    // releases a handle for repurposing
	  void releaseHandle(HandleType handle);

	  // returns the number of handles currently grabbed
	  size_t getNumGrabbedHandles() const;

	protected:

    // class constructor
    HandleManager();

	private:

    static HandleManager* _instance;

    HandleType             _handleCounter;
    std::stack<HandleType> _handleStack;

    std::mutex _handleHandleMutex;

};

//////////////////////////////////////////////////////////////////////////////////////////

class HandleGuard
{
  public:

    // class destructor
	  HandleGuard();

    // class destructor
	  virtual ~HandleGuard();

	  // returns the actual handle
	  const HandleManager::HandleType& getHandle() const;

  private:

    HandleManager::HandleType _handle;

};



} // namespace gloost


#endif // H_GLOOST_HANDLEMANAGER


