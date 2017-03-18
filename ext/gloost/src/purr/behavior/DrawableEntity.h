
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


#ifndef H_PURR_BEHAVIOR_DRAWABLEENTITY
#define H_PURR_BEHAVIOR_DRAWABLEENTITY


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

  namespace { class EventData; }


namespace behavior
{

  // marks a drawable subgraph and stores its type and a appropriate or subtree

class DrawableEntity : public BehaviorBase
{
	public:

    const static std::string BehaviorName;

    /// a std::shared_ptr of an DrawableEntity instance
    typedef std::shared_ptr<DrawableEntity>       shared_ptr;
    typedef std::shared_ptr<const DrawableEntity> const_shared_ptr;

    typedef std::string EntityType;


    // class constructor
    static shared_ptr create(const EntityType& type = "purr::model::Node");

    // class destructor
    virtual ~DrawableEntity();


    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);


    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();


    // prepare the entity for drawing
    virtual void prepareToDraw();


    // returns the type of the drawable entity
    const EntityType& getEntityType() const;

    void setLabelOffset(const gloost::Vector3& labelOffset);

    const gloost::Vector3& getLabelOffset() const;


    //

	protected:

    // class constructor
    DrawableEntity(const EntityType& role);


	private:

	  EntityType _entityType;
	  gloost::Vector3 _labelOffset;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_BEHAVIOR_DRAWABLEENTITY


