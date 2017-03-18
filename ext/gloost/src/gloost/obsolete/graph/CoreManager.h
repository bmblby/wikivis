
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



#ifndef GLOOST_COREMANAGER_H
#define GLOOST_COREMANAGER_H


// gloost includes
#include <gloost/graph/Core.h>

// cpp includes
#include <map>




#define GLOOST_COREMANAGER_DEFAULT_PURECORE "defaultPure"


namespace gloost
{
namespace graph
{


  /// Core container with refcount abilities

class CoreManager
{


public:
  ~CoreManager();


  /// get an instance to the CoreManager
  static CoreManager* get();

  /// copy contructor to avoid assignment to a non reference
  explicit CoreManager(const CoreManager &) {}



  /// Add existing Core to be managed by the CoreManager (refCount +). Returns a unique id for this Core.
  unsigned int addCore(Core* core);
  /// Add existing Core to be managed by the CoreManager and assign a (unique) name for it (refCount +). Returns a unique id for this Core.
  unsigned int addCoreAndAssignName(Core* core, const std::string& name);

  /// Assign a (please unique) name for a core within the core manager (refCount +)
  void assignName(unsigned int coreId, const std::string& name);



  /// check if a core with id exists within the CoreManager, returns the typeId of the core like GLOOST_CORE_MATRIX.
  unsigned int coreExists(unsigned int coreId);

  /// check if a core with id exists within the CoreManager, returns the typeId of the core like GLOOST_CORE_MATRIX.
  unsigned int coreExists(const std::string& coreName);



  /// returns reference to an existing Core object and increment the reference counter (refCount +)
  Core* getCoreReference(unsigned int coreId);

  /// returns reference to an existing Core object and increment the reference counter (refCount +)
  Core* getCoreReference(const std::string& coreName);

  /// returns reference to gloost::Core object WITHOUT incrementing the reference counter
  Core* getCoreWithoutRefcount(unsigned int coreId);

  /// returns reference to gloost::Core object WITHOUT incrementing the reference counter
  Core* getCoreWithoutRefcount(const std::string& coreName);



  /// drops reference to gloost::Core object (refCount -)
  void dropReference(unsigned int coreId);

  /// drops reference to gloost::Core object (refCount -)
  void dropReference(const std::string& coreName);



  /// deletes all unused Cores (... if the reference counter of a core is 0)
  void cleanUp();


  /// returns number of all Cores within the manager
  unsigned int getSize() const;


private:

  ///
  static CoreManager* _theinstance;

  /// internal ID counter for unique IDs
  unsigned int _idCounter;


  /// map to hold the cores
  std::map<unsigned int, Core*> _cores;


  /// map to hold name->id assoziations
  std::map<std::string, unsigned int> _assignMap;



  /// class constructor
  CoreManager();

};


// ostream operator
extern std::ostream& operator<< (std::ostream&, const CoreManager& );



} // namespace graph
}  // namespace gloost



#endif // GLOOST_COREMANAGER_H
