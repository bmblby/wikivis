
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

             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_ACTION_ACTIONBASE
#define H_PURR_ACTION_ACTIONBASE

// gloost system includes
#include <gloost/BitMask.h>


// cpp includes
#include <string>
#include <memory>
#include <unordered_map>


namespace purr
{

  namespace model { class Node; }

namespace action
{

  // CallOrder type and defaults
  typedef std::string CallOrder;
  const static std::string call( "purr::action::call" );
  const static std::string callUpwards(   "purr::action::callUpwards");
  const static std::string callSubnodes(  "purr::action::callSubnodes");
  const static std::string callSiblings(  "purr::action::callSiblings");


  // base class of all action/traverser

class ActionBase : public std::enable_shared_from_this<ActionBase>
{
	public:

	  enum class TraversalHint : char
	  {
      exit = 0, proceed, repeat, repeatParent, done
	  };

    // class destructor
    virtual ~ActionBase();

    // returns the type of the action
    virtual const std::string& getType() const;

    // begins the traversal on a specific node
    virtual void begin(const std::shared_ptr<model::Node>& startNode);

    const gloost::BitMask& getTravMask() const;
    void setTravMask(const gloost::BitMask&);

	protected:

    // class constructor
    ActionBase(const std::string& type);

    /// map of event name -> std::function
    std::unordered_map<std::string,
                       std::function<void(void)> > _traversOrders;

	private:

    std::string     _type;
    gloost::BitMask _travMask;


};


} // namespace action
} // namespace purr


#endif // H_PURR_ACTION_ACTIONBASE


