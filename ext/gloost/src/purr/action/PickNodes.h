
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


#ifndef H_PURR_ACTION_PICKNODES
#define H_PURR_ACTION_PICKNODES



// purr includes
#include <purr/action/ActionBase.h>


// gloost includes
#include <gloost/Point3.h>
#include <gloost/BitMask.h>

// cpp includes
#include <string>
#include <memory>
#include <vector>



namespace gloost { class MouseState; class MatrixStack; class Point3; }


namespace purr
{

namespace action
{

  // base class of all action/traverser

class PickNodes : public ActionBase
{
	public:

    /// a std::shared_ptr of an PickNodes instance
    typedef std::shared_ptr<PickNodes>       shared_ptr;
    typedef std::shared_ptr<const PickNodes> const_shared_ptr;

    // creator
    static shared_ptr create();


    // class destructor
    virtual ~PickNodes();


    // begins the traversal on a specific node
    virtual void beginInorder(const std::shared_ptr<model::Node>& startNode,
                              const gloost::Point3& worldSpaceCoord);


    // returns an inorder vector with shared_ptr to the picked nodes
    std::vector<std::shared_ptr<model::Node> >& getPickedNodes();
    const std::vector<std::shared_ptr<model::Node> >& getPickedNodes() const;


	protected:

    // class constructor
    PickNodes();

    // traversers the tree inorder
    virtual void traverseInorder(const std::shared_ptr<model::Node>& currentNode,
                                 const gloost::Point3& worldSpaceCoord,
                                 gloost::MatrixStack& modelMatrixStack,
                                 int currentDepth);


	private:

	  std::vector<std::shared_ptr<model::Node> > _pickedNodes;

	  gloost::BitMask                            _travMask;




};


} // namespace action
} // namespace purr


#endif // H_PURR_ACTION_PICKNODES


