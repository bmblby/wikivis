
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


#ifndef H_PURR_ACTION_BUILDMESH
#define H_PURR_ACTION_BUILDMESH



// purr includes
#include <purr/action/ActionBase.h>

#include <purr/behavior/DrawableEntity.h>


// gloost includes
#include <gloost/Matrix.h>
#include <gloost/BoundingBox.h>
#include <gloost/UniformTypes.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost { class MouseState; class MatrixStack; }


namespace purr
{

namespace action
{

  // Traverses the graph inorder and builds the vertex attributes to draw the Nodes

class BuildMesh : public ActionBase
{
	public:

    /// a std::shared_ptr of an BuildMesh instance
    typedef std::shared_ptr<BuildMesh>       shared_ptr;
    typedef std::shared_ptr<const BuildMesh> const_shared_ptr;


    // stores the geometry of and a reference to one node
    struct GeometryListEntry
    {
      typedef std::shared_ptr<GeometryListEntry>       shared_ptr;
      typedef std::shared_ptr<const GeometryListEntry> const_shared_ptr;
      static std::shared_ptr<GeometryListEntry> create(){return shared_ptr(new GeometryListEntry());}

      gloost::Matrix                                _modelMatrix;
      gloost::BoundingBox                           _worldSpaceAabb;
      std::shared_ptr<model::Node>               _node;
      float                                         _nodeDepth;
      gloost::vec4                                  _activation = {0.0f, 0.0f, 0.0f, 0.0f};
      std::shared_ptr<behavior::DrawableEntity>    _drawableEntityBehavior;
    };

    typedef std::vector<GeometryListEntry::shared_ptr> GeometryList;

    // creator
    static shared_ptr create();

    // class destructor
    virtual ~BuildMesh();

    // begins the traversal on a specific node
    void begin(const std::shared_ptr<model::Node>& startNode);

    // returns a vector of GeometryListEntries
    GeometryList& getGeometryList();


	protected:

    // class constructor
    BuildMesh();

    // traversers the tree inorder
    virtual void gatherGeometryInorder(const std::shared_ptr<model::Node>& currentNode,
                                       gloost::MatrixStack& modelMatrixStack,
                                       int currentDepth);


	private:

	  GeometryList _geometryList;


};


} // namespace action
} // namespace purr


#endif // H_PURR_ACTION_BUILDMESH


