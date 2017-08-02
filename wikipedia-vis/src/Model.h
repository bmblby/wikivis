#ifndef MODEL_HPP
#define MODEL_HPP

#include <math.h>

//glm
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// graph and layout
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
//layout
#include <boost/graph/breadth_first_search.hpp>
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
#define YELLOW_SUN          {1.0, .99, .0, .9}
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
    size_t mutable level = 0;
    Point mutable position;
    double mutable angle;
    double mutable r_bis_lim;
    double mutable l_bis_lim;
    double mutable r_tan_lim;
    double mutable l_tan_lim;
    double mutable deg_prev_cat;
    double mutable deg_next_cat;
    std::array<float, 4> mutable color;
};

struct EdgeProp {
  EdgeProp(): color(WHITE) {}
  static uint32_t weight;
  std::array<float, 4> mutable color;
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
using EdgeIt = boost::graph_traits<Graph>::edge_iterator;
using EdgePair = std::pair<Edge, bool>;

class Model
{
    public:
    // class constructor
    Model(WikiDB&);

    void initGraph(Category const& root, size_t depth = 2);
    Graph buildDFS(Graph& g, Category const& cat, size_t depth);
    void initIDDFS(Category const& root, size_t depth);
    Graph buildDLS(Graph& g, Category const& cat, Vertex& v, size_t depth);
    std::pair<bool, Vertex> in_graph(Graph& g, Category const& cat) const;

    void expand(Category const& cat);
    std::pair<Vertex, EdgePair>
    add_cat(Graph& g,
                Category const& cat,
                Vertex const& parent,
                std::array<float, 4> color = RED_NODE
    );

    //layouts with BGL
    using PosMap = boost::property_map<Graph, Point CatProp::*>::type;
    PosMap layout_circular(double const& radius);
    PosMap layout_FR();
    PosMap layout_random();
    void write_layout(PosMap pos_map);
    //layout with visitor
    PosMap layout(Category const& cat, size_t width, size_t height, size_t depth);


    //getter
    std::vector<std::pair< glm::vec3,
                         std::array<float, 4> > >
    get_nodes() const;

    std::vector<std::tuple<const glm::vec3,
                         const glm::vec3,
                         const std::array<float, 4>>>
    get_edges() const;

    bool find(std::string const& cat, Category& category) const;
    bool pos2cat(glm::vec3 target, Category& cat) const;
    bool dump_graph(std::string filename) const;

    // Member
    size_t _max_depth;
    Vertex _root;
    Graph _graph;
    WikiDB& _wikidb;

    bool _dirty;

};

} // Namespace vta
#endif // MODEL_HPP
