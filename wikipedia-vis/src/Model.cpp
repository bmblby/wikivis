#include "Model.h"
// debug

namespace vta
{

////////////////////////////////////////////////////////////////////////////////

Model::Model(WikiDB& wikidb)
:
  _dirty(true),
  _wikidb(wikidb),
  _graph()
{}

void
Model::insert_into_graph(Category const& cat, std::vector<Category> children, Graph& g)
{
  typedef  boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef  boost::graph_traits<Graph>::edge_descriptor Edge;
  typedef std::pair<Edge, bool> EdgePair;

  Vertex v0 = add_vertex(g);
  g[v0].index = cat.index;
  g[v0].revid = cat.revid;
  g[v0].title = cat.title;
  for(auto const& child : children) {
    Vertex v1 = add_vertex(g);
    g[v1].index = child.index;
    g[v1].revid = child.revid;
    g[v1].title = child.title;
    EdgePair ep0 = add_edge(v0, v1, g);
  }
  _graph = g;
}

void
Model::build_graph(std::string cat_title, Graph& g, int depth)
{
  Category cat = _wikidb.getCategoryByName(cat_title);

  if(depth > 0) {
    depth--;
    std::vector<Category> children = _wikidb.getCategoryChildren(cat.index);
    for(auto const& it : children) {
      build_graph(std::string(it.title), g, depth);
    }
    insert_into_graph(cat, children, g);
  }
  else {
    std::vector<Category> children = _wikidb.getCategoryChildren(cat.index);
    insert_into_graph(cat, children, g);
  }
}

void
Model::layout_circular(double const& radius)
{
    auto pos_map = get(&CatProp::position, _graph);
    boost::circle_graph_layout(_graph, pos_map, radius);

    typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
        boost::graph_traits<Graph>::vertex_descriptor vertex = *vi;
        _graph[vertex].position[0] = pos_map[vertex][0];
        _graph[vertex].position[1] = pos_map[vertex][1];

        //debug vertices and edges
        // std::cout << "Point: (" << _graph[vertex].position[0]
        //           << ", "       << _graph[vertex].position[1]
        //           << ")" << std::endl;
    }
}

void
Model::layout_FR()
{
    using Topology = boost::circle_topology<boost::mt19937>;
    using Position = Topology::point_type;

    auto pos_map = get(&CatProp::position, _graph);
    Topology topo;

    boost::random_graph_layout(
        _graph,
        pos_map,
        topo
    );

    boost::fruchterman_reingold_force_directed_layout(
        _graph,
        pos_map,
        topo
        // boost::attractive_force([](Graph::edge_descriptor, double k, double d, Graph const&) { return (d*d)/k; })
    );

    //write layout to _graph
    typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
        boost::graph_traits<Graph>::vertex_descriptor vertex = *vi;
        _graph[vertex].position[0] = pos_map[vertex][0];
        _graph[vertex].position[1] = pos_map[vertex][1];
    }
}

void
Model::layout_random()
{
    auto pos_map = get(&CatProp::position, _graph);
    boost::square_topology<boost::mt19937> topo;
    boost::random_graph_layout(_graph, pos_map, topo);

    //write layout to _graph
    typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
        boost::graph_traits<Graph>::vertex_descriptor vertex = *vi;
        _graph[vertex].position[0] = pos_map[vertex][0];
        _graph[vertex].position[1] = pos_map[vertex][1];
    }
}

// template<typename PositionMap>
// void
// Model::write_layout_to_graph(PositionMap position)
// {
//     typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
//     for (boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
//         boost::graph_traits<Graph>::vertex_descriptor vertex = *vi;
//         _graph[vertex].position[0] = position[vertex][0];
//         _graph[vertex].position[1] = position[vertex][1];
//     }
// }


std::vector<std::pair<glm::vec3, std::array<float, 4> > >
Model::get_nodes() const
{
  using NodeIt = boost::graph_traits<Graph>::vertex_iterator;
  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  using Pair = std::pair< glm::vec3,
                          std::array<float, 4> >;
  NodeIt vi, vi_end;
  std::vector<Pair> node_vec;

  std::ofstream file("node_positionst", std::ios::app);
  for(boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
    Vertex vertex = *vi;
    glm::vec3 pos;
    pos[0] = (float)_graph[vertex].position[0];
    pos[1] = (float)_graph[vertex].position[1];
    pos[2] = 0.0f;
    std::array<float, 4> color = _graph[vertex].color;
    Pair node_prop(pos, color);
      node_vec.push_back(node_prop);

    //debug write nodes to file
    static int index = 0;
    int num_vertices = boost::num_vertices(_graph);
    if(index < num_vertices) {
      file << "Category: " << _graph[vertex].title << "\n";
      file << "Index: " << _graph[vertex].index << "\n";
      file << "Revid: " << _graph[vertex].revid << "\n";
      file << "Point: (" << pos[0]
                      << ", " << pos[1]
                      << ", " << pos[2] << ")\n\n";
      index++;
    }
  }

  //   std::cout << "Color: " << _graph[vertex].color[0] << "\n";
  // std::cout << "Number of Points: " << node_vec.size() << "\n";
  return node_vec;
}

std::vector<std::tuple<const glm::vec3, const glm::vec3, const std::array<float, 4> > >
Model::get_edges() const
{
    using EdgeIt = boost::graph_traits<Graph>::edge_iterator;
    using Edge = boost::graph_traits<Graph>::edge_descriptor;
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
    using Tuple = std::tuple<const glm::vec3,
                             const glm::vec3,
                             const std::array<float, 4>>;


    std::vector<Tuple> edge_vec;
    EdgeIt ei, ei_end;
    for(boost::tie(ei, ei_end) = edges(_graph); ei != ei_end; ei++) {
        Edge edge = *ei;
        Vertex source = boost::source(edge, _graph);
        auto prop_source = get(&vta::CatProp::title, _graph, source);
        Vertex target = boost::target(edge, _graph);
        auto prop_target = get(&vta::CatProp::title, _graph, target);
        // std::cout << prop_source << " to " << prop_target << std::endl;
        glm::vec3 source_pos;
        source_pos[0] = _graph[source].position[0];
        source_pos[1] = _graph[source].position[1];
        source_pos[2] = 0.0f;
        glm::vec3 target_pos;
        target_pos[0] = _graph[target].position[0];
        target_pos[1] = _graph[target].position[1];
        target_pos[2] = 0.0f;
        // auto edge_color = _graph[edge].color;
        std::array<float, 4> edge_color =  {.0, .9, .0, .9};
        Tuple tuple(source_pos, target_pos, edge_color);
        edge_vec.push_back(tuple);
    }
    return edge_vec;
}


Category
Model::posToCat(glm::vec3 pos)
{
    auto pos_map = get(&vta::CatProp::position, _graph);
    // auto vertex_pair = vertices(pos_map);
    int cat_index(1);
    Category cat;
    for(auto vp = boost::vertices(_graph); vp.first != vp.second; ++vp.first) {
        auto vertex_iter = vp.first;
        Point p = get(pos_map, *vertex_iter);
            auto distance = glm::distance(pos, glm::vec3(p[0], p[1], 0.0f));
        if(distance < 0.101f) {
            std::cout << "mouse Position: (" <<  pos[0] << ", " << pos[1] << ")\n";
            std::cout << "Vertex Position: (" <<  p[0] << ", " << p[1] << ")\n";
            std::cout << "Distance: (" << distance << ")\n\n";
            cat_index = _graph[*vertex_iter].index;
            cat = _wikidb.getCategory(cat_index);
            std::cout << cat << std::endl;
        }

    }
    cat = _wikidb.getCategory(cat_index);

    return cat;
}

} // Namespace vta
