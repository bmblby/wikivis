
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
#include <purr/model/PurrModel.h>
#include <purr/model/Node.h>
#include <purr/action/CallEvent.h>


// gloost includes
#include <gloost/BinaryFile.h>
#include <gloost/BinaryBundle.h>
#include <gloost/gloostSerial.h>
#include <gloost/gloostHelper.h>
#include <gloost/Matrix.h>
#include <gloost/Point3.h>


// cpp includes
#include <string>
#include <iostream>



namespace purr
{


namespace model
{


/**
  \class   PurrModel

  \brief   tnt::Flow observer, creates entities for a graphical representations of nodes and connections

  \author  Felix Weiszig, Dynardo GmbH Weimar
  \date    january 2014
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief
  \remarks ...
*/

/*static*/
PurrModel::shared_ptr
PurrModel::create()
{
	shared_ptr model(new PurrModel());
  model->setRootNode(Node::create(model, "root"));
	return model;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

PurrModel::PurrModel():
  _rootNode(nullptr),
  _mouseState(nullptr)
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
PurrModel::~PurrModel()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   set/initializes the root node
  \remarks ...
*/

void
PurrModel::setRootNode(const std::shared_ptr<Node>& node)
{
  _rootNode = node;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a container with gui elements
  \remarks ...
*/

std::shared_ptr<Node>
PurrModel::getRootNode()
{
  return _rootNode;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the current mouse state to be accessable by Behaviors
  \remarks ...
*/

/*virtual*/
void
PurrModel::setCurrentMouseState(const std::shared_ptr<const gloost::human_input::MouseState>& mouseState)
{
  _mouseState = mouseState;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the current mouse state to be accessable by Behaviors
  \remarks ...
*/

/*virtual*/
std::shared_ptr<const gloost::human_input::MouseState>
PurrModel::getCurrentMouseState() const
{
  return _mouseState;
}


////////////////////////////////////////////////////////////////////////////////



} // namespace model

} // namespace purr


