
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


#ifndef H_PURR_BEHAVIOR_SUBGRAPHROOT
#define H_PURR_BEHAVIOR_SUBGRAPHROOT


// purr includes
#include <purr/behavior/BehaviorBase.h>


// gloost system includes
#include <gloost/gloostConfig.h>


// cpp includes
#include <string>
#include <memory>


namespace purr
{

  namespace model { class SubgraphDecoratorBase; }


namespace behavior
{

  // stores a ptr to a purr::model::SubgraphDecoratorBase

class SubgraphRoot : public purr::behavior::BehaviorBase
{
	public:

    const static std::string BehaviorName;


    /// a std::shared_ptr of an SubgraphRoot instance
    typedef std::shared_ptr<SubgraphRoot>       shared_ptr;
    typedef std::shared_ptr<const SubgraphRoot> const_shared_ptr;


    // class constructor
    static shared_ptr create(const std::shared_ptr<model::SubgraphDecoratorBase>& model);

    // class destructor
    virtual ~SubgraphRoot();

    // returns for a given base class shared_ptr a shared_ptr of this type or a nullptr
    static shared_ptr castToThisType(std::shared_ptr<BehaviorBase> basePointer);

    // must be called when the behavior is added to a model::Node
    virtual void onBeginBehavior();


    // returns the model::SubgraphDecoratorBase instance
    std::shared_ptr<model::SubgraphDecoratorBase> getDecorator();
    std::shared_ptr<const model::SubgraphDecoratorBase> getDecorator() const;


	protected:

    // class constructor
    SubgraphRoot(const std::shared_ptr<purr::model::SubgraphDecoratorBase>& decorator);


	private:

	  std::shared_ptr<model::SubgraphDecoratorBase> _decorator;


};


} // namespace behavior
} // namespace purr


#endif // H_PURR_GUI_BEHAVIOR_WIDGETROOT


