#ifndef MODEL_HPP
#define MODEL_HPP

//glm
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// graph and layout
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/point_traits.hpp>
#include <boost/random.hpp>

// wikidb
#include <contract.h>
#include <record_not_found.h>
#include <SimPair.h>
#include <wikidb.h>

namespace vta
{
//colors
#define RED_NODE            {.93, .32, .25, .6}
#define PINK                {.98, .12, 1.0, .6}
#define YELLOW_SOFT         {1.0, .98, .36, .6}
#define GREY_SOFT           {.22, .24, .39, .9}
#define WHITE               {.95, .97, 1.0, .5}
#define BLUE_BACK           {.29, .51, .65, .5}

using Point = boost::square_topology<>::point_type;
struct CatProp {
    CatProp(): color(RED_NODE) {}
    uint32_t index;
    uint32_t revid;
    std::string title;
    size_t num_articles;
    size_t num_categories;

    //Layout properties
    enum {white, grey, black, root};
    int tag = white;
    size_t level = 0;
    Point position;
    std::array<float, 4> color;
};

struct EdgeProp {
  EdgeProp(): color(WHITE) {}
  static uint32_t weight;
  std::array<float, 4> color;
};

//TODO:0 choose final graph params
using Graph = boost::adjacency_list<
                 boost::setS
               , boost::vecS
               , boost::bidirectionalS
               , CatProp
               , EdgeProp
               >;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
using VertexIt = boost::graph_traits<Graph>::vertex_iterator;
using Edge = boost::graph_traits<Graph>::edge_descriptor;
using EdgePair = std::pair<Edge, bool>;

class Model
{
    public:
    // class constructor
    Model(WikiDB&);

    void initGraph(Category const& cat, int depth = 2);
    Graph build(Graph& g, Category const& cat, int depth = 1);
    void expand(Category const& cat);

    Vertex add_cat(Graph& g,
                Category const& cat,
                Vertex const& parent,
                std::array<float, 4> color = {.8, .1, .0, .6}
    );

    //layouts
    using PosMap = boost::property_map<Graph, Point CatProp::*>::type;
    PosMap layout_circular(double const& radius);
    PosMap layout_FR();
    PosMap layout_random();
    void write_layout(PosMap pos_map);
    bool dump_graph(Graph& g, std::string filename) const;

    bool find(std::string const& cat, Category& category) const;

    //getter
    std::vector<std::pair< glm::vec3,
                         std::array<float, 4> > >
    get_nodes() const;

    std::vector<std::tuple<const glm::vec3,
                         const glm::vec3,
                         const std::array<float, 4>>>
    get_edges() const;

    bool pos2cat(glm::vec3 target, Category& cat) const;

    // Member
    size_t _max_depth;
    Vertex _root;
    Graph _graph;
    WikiDB& _wikidb;

    bool _dirty;

};

} // Namespace vta
#endif // MODEL_HPP
