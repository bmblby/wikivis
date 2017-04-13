
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


#ifndef H_PURR_BEHAVIOR_ACTIVATE
#define H_PURR_BEHAVIOR_ACTIVATE



// purr includes
#include <purr/behavior/BehaviorBase.h>


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace gloost { class MouseState; class vec4; }

namespace purr
{

namespace behavior
{

  // base class of all behaviors

class Activate : public BehaviorBase
{
	public:

    const static std::string BehaviorName;

//    const static std::string onActivate;
//    const static std::string onActivateSlow;
//    const static std::string onActivateFast;
//
//    const static std::string onDeactivate;
//    const static std::string onDeactivateSlow;
//    const static std::string onDeactivateFast;

    const static std::string onSetTargetColor;
    struct SetTargetColorEventData { gloost::vec4 _color; float _hardness; };


    /// a std::shared_ptr of an Activate instance
    typedef std::shared_ptr<Activate>       shared_ptr;
    typedef std::shared_ptr<const Activate> const_shared_ptr;

    // class constructor
    static shared_ptr create();

    // class destructor
    virtual ~Activate();

    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);



    // CALLBACKS: called once

    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();

    // is called at the beginning of a step
    virtual void frameStep();

    // ...
    void setTarget(const std::shared_ptr<purr::event::EventData>& data);
    void setTarget(const gloost::vec4& targetValue, float hardness);
    void setAll(const gloost::vec4& currentValue, const gloost::vec4& targetValue, float hardness);

    // returns the actvation
    const gloost::vec4& getValue() const;


	protected:

    // class constructor
    Activate();


	private:

	  gloost::vec4 _currentValue;
	  gloost::vec4 _targetValue;
	  float        _hardness;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_ACTIVATE


