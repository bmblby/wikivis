
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


// purr includes
#include <purr/action/CallEvent.h>
#include <purr/model/Node.h>
#include <purr/behavior/BehaviorBase.h>

// gloost system includes
#include <gloost/human_input/MouseState.h>
#include <gloost/util/Timer.h>



// cpp includes
#include <string>
#include <iostream>
#include <thread>



namespace purr
{


namespace action
{


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   traverses the tree inorder and calls event with <eventName> on all nodes behaviors
  \remarks ...
*/

/*static*/
void
CallEvent::callPreorder(const std::shared_ptr<model::Node>& node,
                       const std::string& eventName,
                       const std::shared_ptr<event::EventData>& data)
{
  if (!node)
  {
    return;
  }

  // call on all my behaviors
  for (const auto& behavior : node->getBehaviorsByCallOrder())
  {
    behavior->callEvent(eventName, data);
  }

  // call for all my subnodes
  for (const auto& subNode : node->getSubnodes())
  {
    callPreorder(subNode, eventName, data);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   traverses the tree inorder and calls event with <eventName> on all nodes behaviors
  \remarks ...
*/

/*static*/
void
CallEvent::callPreorderLater(const std::shared_ptr<model::Node>& node,
                             const std::string& eventName,
                             unsigned milliseconds,
                             const std::shared_ptr<event::EventData>& data)
{
  std::thread waitAndCallthread([=]
              {
                gloost::util::Timer::wait(milliseconds);
                callPreorder(node, eventName, data);
              });

  waitAndCallthread.detach();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls an event on all behavior of on node
  \remarks ...
*/

/*static*/
void
CallEvent::call(const std::shared_ptr<model::Node>& node,
                    const std::string& eventName,
                    const std::shared_ptr<event::EventData>& data)
{
  if (!node)
  {
    return;
  }

  // call on all my behaviors
  for (const auto& behavior : node->getBehaviorsByCallOrder())
  {
    behavior->callEvent(eventName, data);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls an event on all behavior of on node
  \remarks ...
*/

/*static*/
void
CallEvent::callLater(const std::shared_ptr<model::Node>& node,
                         const std::string& eventName,
                         unsigned milliseconds,
                         const std::shared_ptr<event::EventData>& data)
{
  std::thread waitAndCallthread([=]
              {
                gloost::util::Timer::wait(milliseconds);
                call(node, eventName, data);
              });

  waitAndCallthread.detach();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls an event on all behavior of all predecessor of node
  \remarks ...
*/

/*static*/
void
CallEvent::callUpwards(const std::shared_ptr<model::Node>& node,
                       const std::string& eventName,
                       const std::shared_ptr<event::EventData>& data)
{
  if (!node || !node->getParentNode() )
  {
    return;
  }

  auto currentNode = node->getParentNode();

  // call on all my behaviors
  for (const auto& behavior : currentNode->getBehaviorsByCallOrder())
  {
    behavior->callEvent(eventName, data);
  }

  // call parent
  callUpwards(currentNode, eventName, data);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls an event on all behavior of all predecessor of node
  \remarks ...
*/

/*static*/
void
CallEvent::callUpwardsLater(const std::shared_ptr<model::Node>& node,
                            const std::string& eventName,
                            unsigned milliseconds,
                            const std::shared_ptr<event::EventData>& data)
{



  if (!node || !node->getParentNode() )
  {
    return;
  }


  std::thread waitAndCallthread([=]
              {
                gloost::util::Timer::wait(milliseconds);
                auto currentNode = node->getParentNode();

                // call on all my behaviors
                for (const auto& behavior : currentNode->getBehaviorsByCallOrder())
                {
                  behavior->callEvent(eventName, data);
                }

                // call parent
                callUpwards(currentNode, eventName, data);
              });

  waitAndCallthread.detach();



}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls an event on all behavior of direct subnodes of node
  \remarks ...
*/

/*static*/
void
CallEvent::callSubnodes(const std::shared_ptr<model::Node>& node,
                        const std::string& eventName,
                        const std::shared_ptr<event::EventData>& data)
{
  if (!node)
  {
    return;
  }

  // call for all my subnodes
  for (const auto& subNode : node->getSubnodes())
  {
    call(subNode, eventName, data);
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   calls an event on all behavior of all siblings of node
  \remarks ...
*/

/*static*/
void
CallEvent::callSiblings(const std::shared_ptr<model::Node>& node,
                        const std::string& eventName,
                        const std::shared_ptr<event::EventData>& data)
{
  if (!node)
  {
    return;
  }

  if (!node->getParentNode())
  {
    return;
  }

  auto& allSubnodes = node->getParentNode()->getSubnodes();

  // call for all siblings
  for (const auto& sibling : allSubnodes)
  {
    if (sibling == node)
    {
      continue;
    }

    call(sibling, eventName, data);
  }
}

////////////////////////////////////////////////////////////////////////////////




















} // namespace model

} // namespace purr


