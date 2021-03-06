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
#define RED_NODE            {0.93, 0.32, 0.25, 0.65}
#define PINK                {0.98, 0.12, 1.0, 0.9}
#define YELLOW              {0.96, 0.80, 0.02, 0.9}
#define WHITE_1               {0.95, 0.97, 1.0, 0.9}
#define BLUE_SOFT           {0.32, 0.60, 0.83, 0.9}
#define GREEN_SOFT          {0.18, 0.67, 0.64, 0.9}
#define BLUE_BACK           {0.059, 0.176, 0.251, 0.2}

// https://color.adobe.com/Dark-UI-color-theme-7388200/edit/?copy=true
#define GREY_0           {0.125, 0.145, 0.161, 0.9}
#define GREY_1           {0.196, 0.220, 0.239, 0.9}
#define GREY_2           {0.298, 0.333, 0.361, 0.9}
#define WHITE_0          {0.953, 0.961, 0.973, 0.9}
#define BLUE_0           {0.0, 0.608, 1.0, 0.9}


using Point = boost::square_topology<>::point_type;
struct CatProp {
    CatProp(): color(BLUE_SOFT) {}
    uint32_t index;
    uint32_t revid;
    std::string title;
    size_t num_articles;
    size_t num_categories;

    //Layout properties
    size_t mutable level = 0;
    int mutable wideness = 0;
    float mutable weight = 0;
    Point mutable pos;
    double mutable angle;
    double mutable r_bis_lim;
    double mutable l_bis_lim;
    double mutable r_tan_lim;
    double mutable l_tan_lim;
    double mutable deg_prev_cat;
    double mutable deg_next_cat;
    glm::vec4 mutable color;
};

struct EdgeProp {
  EdgeProp(): color(GREY_1) {}
  static uint32_t weight;
  std::array<float, 4> mutable color;
};

struct gl_vertex
{
    gl_vertex(glm::vec3 pos, float weight, glm::vec4 col)
    : pos(pos), weight(weight), col(col) {}

    glm::vec3 pos;
    float weight;
    glm::vec4 col;
};

struct gl_edge
{
    gl_edge(glm::vec3 source, glm::vec4 s_col, glm::vec3 target, glm::vec4 t_col)
    : source(source), s_col(s_col), target(target), t_col(t_col) {}
    glm::vec3 source = {0.0f,0.0f,0.0f};
    glm::vec4 s_col = {0.0f,0.0f,0.0f,0.0f};
    glm::vec3 target = {0.0f,0.0f,0.0f};
    glm::vec4 t_col = {0.0f,0.0f,0.0f,0.0f};
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

    // build tree
    void initGraph(Category const& root, size_t depth = 2);
    Graph buildDFS(Graph& g, Category const& cat, size_t depth);
    void initIDDFS(Category const& root, size_t depth);
    Graph buildDLS(Graph& g, Category const& cat, Vertex& v, size_t depth);

    //change tree
    void expand(int depth);
    bool expandCat(Category const& cat);
    void expand_leaves(int depth);
    std::pair<Vertex, EdgePair>
    add_cat(Graph& g,
                Category const& cat,
                Vertex const& parent,
                glm::vec4 color = BLUE_SOFT
    );

    // standard layoutts from BGL
    using PosMap = boost::property_map<Graph, Point CatProp::*>::type;
    PosMap layout_circular(double const& radius);
    PosMap layout_FR();
    PosMap layout_random();
    void write_layout(PosMap pos_map);

    //set layout with visitor
    PosMap layout(Category const& cat, size_t width, size_t height, size_t depth, float radius);
    void relayout(size_t w, size_t h);

    //free tree - EADES
    glm::vec3 pol2cart(float, float);
    std::pair<float, float> cart2pol(glm::vec3 p);
    float tau(float);
    void free_tree(Vertex v, float, float, float);

    //article
    void threshold(float v);
    void focus_cat(uint32_t index, float threshold);

    //getter - for renderer
    std::vector<gl_vertex> get_nodes() const;
    std::vector<gl_edge> get_edges() const;

    //util
    std::pair<bool, Vertex> in_graph(Graph& g, Category const& cat) const;
    std::pair<bool, Vertex> in_graph(Graph& g, uint32_t index) const;
    std::pair<uint32_t, float> fill_data(Category const& cat, Vertex v);
    float mapping(float) const;
    bool find(std::string const& cat, Category& category) const;
    bool pos2cat(glm::vec3 target, Category& cat) const;
    bool dump_graph(std::string filename) const;
    void print_comp(bool local, bool global) const;

    // Member
    float _threshold;
    size_t _max_depth;
    float _r;
    Vertex _root;
    Graph _graph;
    WikiDB& _wikidb;

    //data structures
    std::map<uint32_t, std::vector<SimPair>> _simM;
    //attetion mapping from index to vertices in graph
    std::multimap<Vertex, uint32_t>  _cat2art;
    std::multimap<uint32_t, Vertex>  _art2cat;
    std::set<uint32_t> _categories;
    std::multimap<uint32_t, SimPair> _local_comp;
    std::multimap<uint32_t, SimPair> _global_comp;
    bool _dirty;

};

} // Namespace vta
#endif // MODEL_HPP
