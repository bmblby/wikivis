
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



#ifndef H_GLOOST_UNIQUE_GENERIC_MANAGER
#define H_GLOOST_UNIQUE_GENERIC_MANAGER



// gloost system includes
#include <gloost/util/GenericManager.h>
#include <gloost/gloostConfig.h>
#include <gloost/Vector3.h>
#include <gloost/gloostMath.h>
#include <gloost/BinaryFile.h>
#include <gloost/BoundingBox.h>


// cpp includes
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include <memory>
//#include <boost/thread/mutex.hpp>
#include <mutex>
#include <atomic>



namespace gloost
{
namespace util
{


/**
  \class   UniqueGenericManager

  \brief   A Singleton gloost::util::GenericManager for each type

  \author  Felix Weiszig, dynardo GmbH
  \date    August 2013
  \remarks \code
            // define handles globaly or pass them arround throu your application
            gloost::gloostId valueHandle_A = 0;
            gloost::gloostId valueHandle_B = 0;
            gloost::gloostId valueHandle_C = 0;
            gloost::gloostId valueHandle_D = 0;

            // get the UniqueGenericManager instances for two types
            {
              // get the one for float
              auto mrFloat = gloost::util::UniqueGenericManager<float>::get();
              valueHandle_A = mrFloat->add(new float(6.5f));
              valueHandle_B = mrFloat->add(new float(4.2f));

              // get the one for std::string
              auto mrString = gloost::util::UniqueGenericManager<std::string>::get();
              valueHandle_C = mrString->add(new std::string("Thats"));
              valueHandle_C = mrString->add(new std::string("Manageable"));
            }


            // somwhere else: print content a change value behind valueHandle_A
            {
              auto mrFloat = gloost::util::UniqueGenericManager<float>::get();
              std::cerr << std::endl << "valueHandle_A: " << *mrFloat->get(valueHandle_A);
              std::cerr << std::endl << "valueHandle_B: " << *mrFloat->get(valueHandle_B);
              mrFloat->replace(valueHandle_A, new float(100.0f));


              auto mrString = gloost::util::UniqueGenericManager<std::string>::get();
              std::cerr << std::endl << "valueHandle_C: " << *mrString->get(valueHandle_C);
              std::cerr << std::endl << "valueHandle_C: " << *mrString->get(valueHandle_C);
            }

            // somwhere else: print floats
            {
              auto mrFloat = gloost::util::UniqueGenericManager<float>::get();
              std::cerr << std::endl << "valueHandle_A: " << *mrFloat->get(valueHandle_A);
              std::cerr << std::endl << "valueHandle_B: " << *mrFloat->get(valueHandle_B);
            }
  \endcode
*/


template <class managedType>
class UniqueGenericManager : public GenericManager<managedType>
{

protected:

  /// class constructor

  UniqueGenericManager():
    GenericManager<managedType>()
  {

  }

public:

  // returns the UniqueGenericManager instance for a specific type
  static UniqueGenericManager* get()
  {
    static UniqueGenericManager* instance = new UniqueGenericManager<managedType>();;
    return instance;
  }

  ///////////////////////////////////////////////////////////////////////////

  /**
    \brief  returns the item associated with gid or a nullptr
  */

  virtual
  std::shared_ptr<managedType>
  get(gloostId gid)
  {
    return GenericManager<managedType>::get(gid);
  }

  ///////////////////////////////////////////////////////////////////////////

  /**
    \brief  returns the item associated with alias or a nullptr
  */

  virtual
  std::shared_ptr<managedType>
  get(const std::string& alias)
  {
    return GenericManager<managedType>::get(alias);
  }

public:

  ///////////////////////////////////////////////////////////////////////////


  /// class destructor

  virtual ~UniqueGenericManager()
  {
  }
  ///////////////////////////////////////////////////////////////////////////

protected:

};

} // namespace util
} // namespace gloost

#endif // H_GLOOST_UNIQUE_GENERIC_MANAGER
