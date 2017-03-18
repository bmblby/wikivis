
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

#ifndef H_PURR_VISUALIZE_VISNODE
#define H_PURR_VISUALIZE_VISNODE


// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <memory>

namespace purr
{
  namespace model { class Node; }

namespace visualize
{
  //  VisNode template

class VisNode : public std::enable_shared_from_this<VisNode>
{
	public:

    /// a std::shared_ptr of an VisNode instance
    typedef std::shared_ptr<VisNode>       shared_ptr;
    typedef std::shared_ptr<const VisNode> const_shared_ptr;

    // class factoy
    static shared_ptr create(const std::shared_ptr< model::Node >&  purrNode );

    // class destructor
	  virtual ~VisNode();

    // inits the VisNode
	  void init();

    // sets the depth of the node
	  void setDepth(unsigned depth);
	  unsigned getDepth();

	  //
	  void attachSubnode(const shared_ptr& subNode);
	  std::shared_ptr<VisNode> getParent();

	  std::shared_ptr< model::Node > getPurrNode();

	  void setPosition( const gloost::Point3& pos );
	  void setSize( const gloost::Vector3& size );


	protected:

    // class constructor
    VisNode(const std::shared_ptr< model::Node >&  purrNode);

	private:

    std::shared_ptr< model::Node > _purrNode;
    unsigned                       _nodeDepth;
    shared_ptr                     _parentVisNode;
    std::vector<shared_ptr>        _subVisNode;

    gloost::Point3 _position;
    gloost::Point3 _size;

};

} // namespace visualize
} // namespace purr

#endif // H_PURR_VISUALIZE_VISNODE

