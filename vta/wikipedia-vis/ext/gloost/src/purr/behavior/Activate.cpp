
/*
             __ __ __
            |  |__|  |--.-----.--.--.----.----.         )\._.,--....,'``.
            |  |  |  _  |  _  |  |  |   _|   _|        /;   _.. \   _\  (`._ ,.
            |__|__|_____|   __|_____|__| |__|         `----(,_..'--(,_..'`-.;.'
                        |__|
*/


// purr includes
#include <purr/behavior/Activate.h>
#include <purr/model/PurrModel.h>
#include <purr/action/ActionBase.h>
#include <purr/action/CallEvent.h>
#include <purr/model/Node.h>
#include <purr/event/common.h>

// gloost includes
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <iostream>



namespace purr
{


namespace behavior
{


/**
  \class   Activate

  \brief   base class of all behaviors

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////

  /*static*/ const std::string Activate::BehaviorName = std::string("purr::behavior::Activate");

//  /*static*/ const std::string Activate::onActivate       = std::string("purr::behavior::Activate::onActivate");
//  /*static*/ const std::string Activate::onActivateSlow   = std::string("purr::behavior::Activate::onActivateSlow");
//  /*static*/ const std::string Activate::onActivateFast   = std::string("purr::behavior::Activate::onActivateFast");
//
//  /*static*/ const std::string Activate::onDeactivate     = std::string("purr::behavior::Activate::onDeactivate");
//  /*static*/ const std::string Activate::onDeactivateSlow = std::string("purr::behavior::Activate::onDeactivateSlow");
//  /*static*/ const std::string Activate::onDeactivateFast = std::string("purr::behavior::Activate::onDeactivateFast");

  /*static*/ const std::string Activate::onSetTargetColor = std::string("purr::behavior::Activate::onSetTargetColor");

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   class factory
  \remarks ...
*/

/*static*/
Activate::shared_ptr
Activate::create()
{
	return shared_ptr(new Activate());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Activate::Activate():
  BehaviorBase(Activate::BehaviorName),
  _currentValue({0.0f, 0.0f, 0.0f, 1.0f}),
  _targetValue({0.0f, 0.0f, 0.0f, 1.0f}),
  _hardness(0.2)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Activate::~Activate()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
  \remarks ...
*/

/*static*/
Activate::shared_ptr
Activate::castToThisType(std::shared_ptr<BehaviorBase> basePointer)
{
  return std::dynamic_pointer_cast<Activate>(basePointer);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   must be called when the behavior is added to a Node
  \remarks ...
*/

/*virtual*/
void
Activate::onBeginBehavior()
{
  mapEvent(purr::event::onMouseDown,
           std::bind(&Activate::setAll,
                     this,
                     gloost::vec4(0.0f, 0.14f, 0.0f, 1.0f),
                     gloost::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     0.1f));
  mapEvent(purr::event::onMouseUp,
           std::bind(&Activate::setAll,
                     this,
                     gloost::vec4(0.14f, 0.0f, 0.0f, 1.0f),
                     gloost::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     0.025f));
  mapEvent(purr::event::onMouseDrop,
           std::bind(&Activate::setAll,
                     this,
                     gloost::vec4(0.0f, 0.0f, 0.16f, 1.0f),
                     gloost::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     0.1f));
  mapEvent(purr::event::onMouseEnter,
           std::bind(&Activate::setAll,
                     this,
                     gloost::vec4(0.14f, 0.14f, 0.0f, 1.0f),
                     gloost::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     0.1f));

  mapEvent(purr::event::onMouseDrag,
           std::bind(&Activate::setAll,
                     this,
                     gloost::vec4(0.0f, 0.14f, 0.14f, 1.0f),
                     gloost::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     0.1f));

//  // mapping an overloaded methode
//  mapEvent(onSetTargetColor,
//           std::bind((void(Activate::*)(const std::shared_ptr<purr::event::EventData>&))&Activate::setTarget,
//                     this,
//                     std::placeholders::_1));

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the beginning of a step
  \remarks ...
*/

/*virtual*/
void
Activate::frameStep()
{
  std::lock_guard<std::mutex> lock(getBaseMutex());

  auto difference = _targetValue - _currentValue;

  if ( std::abs(difference.r+difference.g+difference.b+difference.a) > 0.01f)
  {
    _currentValue += difference * _hardness;

//    action::CallEvent::callPreorder(getNode()->getRoot(), "needRefresh");


  }
  else
  {
    _currentValue = _targetValue;
    unmapEvent(purr::event::onExitFrame);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual*/
void
Activate::setTarget(const std::shared_ptr<purr::event::EventData>& data)
{
  if (data)
  {
    auto targetColorData = event::GenericEventData<SetTargetColorEventData>::castToThisType(data);
    if (targetColorData)
    {
      setTarget(targetColorData->getContent()->_color, targetColorData->getContent()->_hardness);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks ...
*/

/*virtual*/
void
Activate::setTarget(const gloost::vec4& targetValue, float hardness)
{
  std::lock_guard<std::mutex> lock(getBaseMutex());

  _targetValue  = targetValue;
  _hardness     = hardness;

  mapEvent(purr::event::onExitFrame,
           std::bind(&Activate::frameStep, this));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   is called at the beginning of a step
  \remarks ...
*/

/*virtual*/
void
Activate::setAll(const gloost::vec4& currentValue, const gloost::vec4& targetValue, float hardness)
{
  std::lock_guard<std::mutex> lock(getBaseMutex());

  _currentValue = currentValue;
  _targetValue  = targetValue;
  _hardness     = hardness;

  mapEvent(purr::event::onExitFrame,
           std::bind(&Activate::frameStep, this));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the actvation
  \remarks ...
*/

/*virtual*/
const gloost::vec4&
Activate::getValue() const
{
  return _currentValue;
}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


