
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
#include <gloost/graph/CoreManager.h>
#include <gloost/graph/PureCore.h>

/// cpp/std includes
#include <iostream>
#include <vector>


namespace gloost
{
namespace graph
{



 /*static*/
 CoreManager* CoreManager::_theinstance = 0;

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

CoreManager::CoreManager():
    _idCounter(0),
    _cores(),
    _assignMap()
{
  addCoreAndAssignName(new PureCore(),
                       "defaultPure");
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

CoreManager::~CoreManager()
{
  getCoreWithoutRefcount(0)->dropReference();
  cleanUp();
}


///////////////////////////////////////////////////////////////////////////////


  /// get an instance to the CoreManager

CoreManager*
CoreManager::get()
{
  if(!_theinstance)
  {
    _theinstance = new CoreManager();
  }

  return _theinstance;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// Add existing Core to be managed by the CoreManager (refCount +)

unsigned int
CoreManager::addCore(Core* core)
{
  ++_idCounter;
  _cores[_idCounter] = core;

  core->takeReference();

  return _idCounter;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// Add existing Core to be managed by the CoreManager and assign a (unique) name for it (refCount +)

unsigned int
CoreManager::addCoreAndAssignName(Core* core, const std::string& name)
{
  unsigned int coreId = addCore(core);

  assignName(coreId, name);

  return coreId;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// Assign a (unique) name for a core within the core manager (refCount +)

void
CoreManager::assignName(unsigned int coreId, const std::string& name)
{
  _assignMap[name] = coreId;
}




//////////////////////////////////////////////////////////////////////////////////////////



  /// check if a core with id exists within the CoreManager, returns the typeId of the core like GLOOST_CORE_MATRIX.

unsigned int
CoreManager::coreExists(unsigned int coreId)
{
  std::map<unsigned int, Core*>::iterator pos = _cores.find(coreId);

  if (pos != _cores.end())
  {
    return (*pos).second->getType();
  }

  return GLOOST_CORE_ERROR;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// check if a core with id exists within the CoreManager, returns the typeId of the core like GLOOST_CORE_MATRIX.

unsigned int
CoreManager::coreExists(const std::string& coreName)
{
  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);

  if (pos != _assignMap.end())
  {
    return _cores[(*pos).second]->getType();
  }

  return GLOOST_CORE_ERROR;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// returns reference to an existing Core object and increment the reference counter (refCount +)

Core*
CoreManager::getCoreReference(unsigned int id)
{
  std::map<unsigned int, Core*>::iterator pos = _cores.find(id);

  if (pos != _cores.end())
  {
    /// (refCount +)
    (*pos).second->takeReference();
    return (*pos).second;
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in CoreManager::getCoreReference(unsigned int id)):";
  std::cout<< std::endl << "         Could not find existing core with id = " << id << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           CoreManager::coreIdExists(unsigned int coreId)";
  std::cout<< std::endl << "         if you are unsure if a core exists";
  std::flush(std::cout);

  return getCoreReference(GLOOST_COREMANAGER_DEFAULT_PURECORE);
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// returns reference to an existing Core object and increment the reference counter (refCount +)

Core*
CoreManager::getCoreReference(const std::string& coreName)
{
  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);

  if (pos != _assignMap.end())
  {
    unsigned int id = (*pos).second;
    /// (refCount +)
    _cores[id]->takeReference();
    return _cores[id];
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in CoreManager::getCoreReference(const std::string& coreName)):";
  std::cout<< std::endl << "         Could not find existing core with name = " << coreName << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           CoreManager::coreIdExists(const std::string& coreName)";
  std::cout<< std::endl << "         if you are unsure if a core exists";
  std::flush(std::cout);

  return getCoreReference(GLOOST_COREMANAGER_DEFAULT_PURECORE);
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// returns reference to gloost::Core object WITHOUT incrementing the reference counter

Core*
CoreManager::getCoreWithoutRefcount(unsigned int id)
{
  std::map<unsigned int, Core*>::iterator pos = _cores.find(id);

  if (pos != _cores.end())
  {
    /// (refCount +)
    (*pos).second->takeReference();
    return (*pos).second;
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in CoreManager::getCoreWithoutRefcount(unsigned int id):";
  std::cout<< std::endl << "         Could not find existing core with id = " << id << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           CoreManager::coreIdExists(unsigned int coreId)";
  std::cout<< std::endl << "         if you are unsure if a core exists";
  std::flush(std::cout);

  return getCoreReference(GLOOST_COREMANAGER_DEFAULT_PURECORE);
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// returns reference to an existing Core object and incrementing the reference counter (refCount +)

Core*
CoreManager::getCoreWithoutRefcount(const std::string& coreName)
{
  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);

  if (pos != _assignMap.end())
  {
    unsigned int id = (*pos).second;
    return _cores[id];
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in CoreManager::getCoreWithoutRefcount(const std::string& coreName):";
  std::cout<< std::endl << "         Could not find existing core with name = " << coreName << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           CoreManager::coreIdExists(const std::string& coreName)";
  std::cout<< std::endl << "         if you are unsure if a core exists";
  std::flush(std::cout);

  return getCoreReference(GLOOST_COREMANAGER_DEFAULT_PURECORE);
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// drops reference to gloost::Core object (refCount -)

void
CoreManager::dropReference(unsigned int id)
{
  std::map<unsigned int, Core*>::iterator pos = _cores.find(id);

  if (pos != _cores.end())
  {
    /// (refCount -)
    (*pos).second->dropReference();
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "Warning from CoreManager::dropReference(unsigned int id):";
  std::cout<< std::endl << "             Could not find existing core with id = " << id << " !";
  std::flush(std::cout);

  return;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// drops reference to gloost::Core object (refCount -)

void
CoreManager::dropReference(const std::string& coreName)
{
  std::map<std::string, unsigned int>::iterator pos = _assignMap.find(coreName);

  if (pos != _assignMap.end())
  {
    unsigned int id = (*pos).second;
    _cores[id]->dropReference();
    return;
  }

  std::cout<< std::endl;
  std::cout<< std::endl << "ERROR in CoreManager::getCoreWithoutRefcount(const std::string& coreName):";
  std::cout<< std::endl << "         Could not find existing core with name = " << coreName << " !";
  std::cout<< std::endl << "         Use";
  std::cout<< std::endl << "           CoreManager::coreIdExists(const std::string& coreName)";
  std::cout<< std::endl << "         if you are unsure if a core exists";
  std::flush(std::cout);

  return;
}


//////////////////////////////////////////////////////////////////////////////////////////


  /// deletes all unused Cores (... if the reference counter of a core is 0)

void
CoreManager::cleanUp()
{

  std::map<unsigned int, Core*>::iterator coreIt    = _cores.begin();
  std::map<unsigned int, Core*>::iterator coreEndIt = _cores.end();

  for (; coreIt != coreEndIt; ++coreIt)
  {

    /// erase and delete the core if it is not referenced. Hopefully the resources
    /// used by the core like shaders, FBOs or UniformSets are referenced from somewhere
    /// else, since they will not be deleted
    if ( !(*coreIt).second->isReferenced())
    {
      unsigned int id = (*coreIt).first;

      delete (*coreIt).second;
      _cores.erase( coreIt );


      /// remove all entries from name map
      std::map<std::string, unsigned int>::iterator nameIt    = _assignMap.begin();
      std::map<std::string, unsigned int>::iterator nameEndIt = _assignMap.end();

      for (; nameIt != nameEndIt; ++nameIt)
      {
        if ( (*nameIt).second == id   )
        {
          _assignMap.erase(nameIt);
        }
      }


    }
  }



}


//////////////////////////////////////////////////////////////////////////////////////////


  /// returns number of all Cores within the manager

unsigned int
CoreManager::getSize() const
{
  return _cores.size();
}


///////////////////////////////////////////////////////////////////////////////


/* extern */
std::ostream&
operator<< (std::ostream& os, const CoreManager& tm)
{

  os << std::endl << "CoreManager" << std::endl << "{  nnnnnnooooottt implemented...";
//  os << std::endl << "    size:    " << tm.getSize();
//
//  os << std::endl;
//
//  tm.printTextureInfo();
//
//  os << std::endl;
//
//  os << std::endl << "} // CoreManager" << std::endl;
//  std::flush(os);
//
  return os;
}


} // namespace graph
} // namespace gloost

