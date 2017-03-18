
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


#ifndef H_PURR_BEHAVIOR_CENTERWITHINPARENT
#define H_PURR_BEHAVIOR_CENTERWITHINPARENT



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

namespace purr
{

namespace behavior
{

  // base class of all behaviors

class CenterWithinParent : public BehaviorBase
{
	public:


    /// a std::shared_ptr of an CenterWithinParent instance
    typedef std::shared_ptr<CenterWithinParent>       shared_ptr;
    typedef std::shared_ptr<const CenterWithinParent> const_shared_ptr;

    // class constructor
    static shared_ptr create(float hardness = 1.0f, float floating = 0.0f);

    // class destructor
    virtual ~CenterWithinParent();

    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);



    // CALLBACKS: called once

    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();

    // is called at the beginning of a step
    virtual void enterFrame();



	protected:

    // class constructor
    CenterWithinParent(float hardness, float floating);

    gloost::Vector3 _speed;
    float           _hardness;
    float           _floating;

    // ...
    void moveToParentsCenter();


	private:

	  /// ...


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_CENTERWITHINPARENT


