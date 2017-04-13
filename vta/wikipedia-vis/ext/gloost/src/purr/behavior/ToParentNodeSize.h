
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_BEHAVIOR_TOPARENTNODESIZE
#define H_PURR_BEHAVIOR_TOPARENTNODESIZE



// purr includes
#include <purr/behavior/BehaviorBase.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/human_input/MouseState.h>


// cpp includes
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace gloost { class MouseState; }
namespace purr {}

namespace purr
{

 namespace model { class EventData; }

namespace behavior
{

  // ...

class ToParentNodeSize : public purr::behavior::BehaviorBase
{
	public:

    const static std::string BehaviorName;

    const static std::string setActive;
    const static std::string setInactive;

    /// a std::shared_ptr of an ToParentNodeSize instance
    typedef std::shared_ptr<ToParentNodeSize>       shared_ptr;
    typedef std::shared_ptr<const ToParentNodeSize> const_shared_ptr;

    // class constructor
    static shared_ptr create(unsigned axis = 0);

    // class destructor
    virtual ~ToParentNodeSize();

    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);



    // CALLBACKS: called once

    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();


    virtual void resizeByParentsSize();

    //

	protected:

    // class constructor
    ToParentNodeSize(unsigned axis);


	private:

	  unsigned _axis;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_TOPARENTNODESIZE


