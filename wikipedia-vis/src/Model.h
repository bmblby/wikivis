#ifndef MODEL_HPP
#define MODEL_HPP

//glm
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// graph and layout
#include <boost/graph/adjacency_list.hpp>
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
using Point = boost::square_topology<>::point_type;
struct CatProp {
    CatProp(): color({.8, .1, .0, .5}) {}
    uint32_t index;
    uint32_t revid;
    std::string title;

    //Layout properties
    // enum {unused, parent};
    int tag;
    Point position;
    std::array<float, 4> color;
};

struct EdgeProp {
  EdgeProp(): color({.0, .7, .0, .7}) {}
  static uint32_t weight;
  std::array<float, 4> color;
};

//TODO:0 choose final graph params
using Graph = boost::adjacency_list<
                 boost::setS
               , boost::vecS
               , boost::undirectedS
               , CatProp
               , EdgeProp
               >;

  class Model
  {
    public:
      // class constructor
      Model(WikiDB&);

      Graph graph(Category const& cat, int depth = 2);
      Graph build(Graph& g, Category const& cat, int depth = 1);
    //   void insert_into_graph(Category const&, std::vector<Category>, Graph& g);
    //   void recursive_build(Graph& g, Category const& cat, int depth = 1);

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

      Category posToCat(glm::vec3 target);

      // Member
      std::vector<Graph> _graphs;
      Graph _graph;
      WikiDB& _wikidb;

      bool _dirty;

  };

} // Namespace vta
#endif // MODEL_HPP
