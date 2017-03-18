
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



#ifndef H_GLOOST_GENERIC_MANAGER
#define H_GLOOST_GENERIC_MANAGER



// gloost system includes
#include <gloost/gloostConfig.h>


// cpp includes
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <atomic>



namespace gloost
{
namespace util
{


/**
  \class   GenericManager

  \brief   template class, stores a set of items and providing unique handles as well as
           reference countig

  \author  Felix Weiszig, dynardo GmbH
  \date    July 2013
  \remarks ...
*/


template <class managedType>
class GenericManager
{
public:

  // mutex container stores a mutex if lock(gloostId id) was called
  typedef boost::unordered_map< gloostId, std::mutex> MutexContainer;

  /// container type for managed typ
  typedef std::unordered_map< gloostId, std::shared_ptr<managedType> > ManagedTypeContainer;

  // container type for string aliases of stored entities
  typedef std::unordered_map< std::string, gloost::gloostId> AliasManagedTypeContainer;

  /// class constructor

  GenericManager():
    _gids(1u),
    _items(),
    _aliaseMap()
  {

  }

  ///////////////////////////////////////////////////////////////////////////


  /// class destructor

  virtual ~GenericManager()
  {
    _items.clear();
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  adds an item and returns an gid for it;
  */

  virtual
  gloostId
  add(managedType* item)
  {
    ++_gids;
    gloostId newId = (gloostId)_gids;
    std::lock_guard<std::mutex> lock(_items_change_mutex);
    {
      _items[newId] = std::shared_ptr<managedType>(item);
    }
    return newId;
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  adds an item and returns an gid for it;
  */

  virtual
  gloostId
  add(const std::shared_ptr<managedType>& itemSharedPtr)
  {
    ++_gids;
    gloostId newId = (gloostId)_gids;
    std::lock_guard<std::mutex> lock(_items_change_mutex);
    {
      _items[newId] = itemSharedPtr;
    }
    return newId;
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  replaces an item and returns true if that worked
  */

  virtual
  bool
  replace(gloostId gid, managedType* item)
  {
    _items_change_mutex.lock();
    {
      auto pos = _items.find(gid);
      if (pos != _items.end())
      {
        pos->second = std::shared_ptr<managedType>(item);
        _items_change_mutex.unlock();
        return true;
      }
      _items_change_mutex.unlock();
      return false;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  replaces an item and returns true if that worked
  */

  virtual
  bool
  replace(gloostId gid, const std::shared_ptr<managedType>& itemSharedPtr)
  {
    _items_change_mutex.lock();
    {
      auto pos = _items.find(gid);
      if (pos != _items.end())
      {
        pos->second = itemSharedPtr;
        _items_change_mutex.unlock();
        return true;
      }
      _items_change_mutex.unlock();
      return false;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  removes an item and returns true if that worked
  */

  virtual
  bool
  remove(gloostId gid)
  {
    _items_change_mutex.lock();
    {
      auto pos = _items.find(gid);
      if (pos != _items.end())
      {
        _items.erase(gid);
        remove_aliaseMap(gid);
        _items_change_mutex.unlock();
        return true;
      }
      _items_change_mutex.unlock();
      return false;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  returns the item associated with gid or a nullptr
  */

  virtual
  std::shared_ptr<managedType>
  get(gloostId gid)
  {
    if (!gid)
    {
      return nullptr;
    }

    _items_change_mutex.lock();
    {
      auto pos = _items.find(gid);
      if (pos != _items.end())
      {
        std::shared_ptr<managedType> foundItem = (pos->second);

        _items_change_mutex.unlock();
        return foundItem;
      }
      _items_change_mutex.unlock();
      return nullptr;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  returns the item associated with gid or a nullptr
  */

  virtual
  std::shared_ptr<managedType>
  get(const std::string& alias)
  {
    _items_change_mutex.lock();
    {
      auto aliasGidIt = _aliaseMap.find(alias);
      if (aliasGidIt == _aliaseMap.end())
      {
        _items_change_mutex.unlock();
        return nullptr;
      }

      auto gid =  (*aliasGidIt).second;
      auto gidItemIt = _items.find(gid);
      if (gidItemIt != _items.end())
      {
        std::shared_ptr<managedType> foundItem = (gidItemIt->second);

        _items_change_mutex.unlock();
        return foundItem;
      }

      _items_change_mutex.unlock();
      return nullptr;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  returns true if the manager contains the gid
  */

  virtual
  bool
  contains(gloostId gid)
  {
    _items_change_mutex.lock();
    {
      auto pos = _items.find(gid);
      if (pos != _items.end())
      {
        _items_change_mutex.unlock();
        return true;
      }
      _items_change_mutex.unlock();
      return false;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  returns true if the manager contains the alias
  */

  virtual
  bool
  contains(const std::string& alias)
  {
    _items_change_mutex.lock();
    {
      auto pos = _aliaseMap.find(alias);
      if (pos != _aliaseMap.end())
      {
        _items_change_mutex.unlock();
        return true;
      }
      _items_change_mutex.unlock();
      return false;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  assigns an alias std::string to an item gid
  */

  virtual
  void
  alias(const std::string& alias, gloostId gid)
  {
    if (get(gid) != nullptr)
    {
      _aliaseMap[alias] = gid;
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  removes an alias
  */

  virtual
  void
  remove_alias(const std::string& alias)
  {
    auto pos = _aliaseMap.find(alias);
    if (pos != _aliaseMap.end())
    {
      _aliaseMap.erase(pos);
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  removes all aliases of a gid ( works in O(number of stored aliases) )
  */

  virtual
  void
  remove_aliaseMap(gloostId gid)
  {
    auto it = _aliaseMap.begin();
    while (it != _aliaseMap.end())
    {
      if (it->second == gid)
      {
        it = _aliaseMap.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  returns a reference to the internal container
  */

  virtual
  ManagedTypeContainer&
  getContainer()
  {
    return _items;
  }


  ///////////////////////////////////////////////////////////////////////////


  /**
    \brief  returns a reference to the internal container
  */

  virtual
  const ManagedTypeContainer&
  getContainer() const
  {
    return _items;
  }


  ///////////////////////////////////////////////////////////////////////////

#if 0
  /**
    \brief  locks the resource for congruent access by locking the associated mutex
  */

  virtual
  void
  lock(gloostId gid)
  {
    if (contains(gid))
    {
      _mutexMap[gid].lock();
    }
  }

  ///////////////////////////////////////////////////////////////////////////

  /**
    \brief  locks the resource for congruent access by locking the associated mutex
  */

  virtual
  void
  unlock(gloostId gid)
  {
    auto it = _mutexMap.find(gid);
    if (it != _mutexMap.end())
    {
      it->second.unlock();
      return;
    }
  }
#endif

  ///////////////////////////////////////////////////////////////////////////

  /**
    \brief locks a resource for congruent access by locking the associated mutex
  */

  virtual
  std::shared_ptr< std::lock_guard< std::mutex> >
  createLockGuard(gloostId gid)
  {
    std::lock_guard<std::mutex> lock(_createLockGuardsMutex);
    if (contains(gid))
    {
      return std::shared_ptr<std::lock_guard<std::mutex> >(new std::lock_guard<std::mutex>(_mutexMap[gid]));
    }
    return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  /**
    \brief locks multiple resources for congruent access by locking the associated mutex for each GID. This
           will also handle multiple references to a single ressource
  */

  virtual
  std::vector< std::shared_ptr< std::lock_guard< std::mutex> > >
  createLockGuards(std::unordered_set<gloostId> gids)
  {
    std::lock_guard<std::mutex> lock(_createLockGuardsMutex);
    std::vector< std::shared_ptr< std::lock_guard< std::mutex> > > lockGuards;
    for (const auto& gid : gids)
    {
      if (contains(gid))
      {
        lockGuards.push_back( std::shared_ptr<std::lock_guard<std::mutex> >(new std::lock_guard<std::mutex>(_mutexMap[gid])) );
      }
    }
    return lockGuards;
  }

  ///////////////////////////////////////////////////////////////////////////

  protected:

    // ...

  private:

    // gid counter
    std::atomic_uint     _gids;
    ManagedTypeContainer _items;
    std::mutex           _items_change_mutex;

    AliasManagedTypeContainer _aliaseMap;

    MutexContainer _mutexMap;
    /// mutex secures that only one client can create lockguards at a given point to prevent deadlocks
    std::mutex _createLockGuardsMutex;

};


} // namespace util
} // namespace gloost


#endif // H_GLOOST_GENERIC_MANAGER


