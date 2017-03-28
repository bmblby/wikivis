#include "Model.h"
// debug

namespace vta
{

using PosMap = boost::property_map<Graph, Point CatProp::*>::type;
////////////////////////////////////////////////////////////////////////////////

Model::Model(WikiDB& wikidb)
:
  _dirty(true),
  _wikidb(wikidb),
  _graph()
{}

Graph
Model::graph(Category const& cat, int depth)
{
   Graph g;
   // recursive_build(g, cat, depth);
   g = build(g, cat, depth);
   std::cout << "number of vertices: " << num_vertices(g) << "\n";
   return g;
}

Graph
Model::build(Graph& g, Category const& cat, int depth)
{
  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  using VertexIt = boost::graph_traits<Graph>::vertex_iterator;
  using Edge = boost::graph_traits<Graph>::edge_descriptor;
  using EdgePair = std::pair<Edge, bool>;
  enum {used, unused, parent, root};
  int static max_depth = depth;

  if(depth == 0) {
    Vertex v = add_vertex(g);
    g[v].index = cat.index;
    g[v].revid = cat.revid;
    g[v].title = cat.title;
    g[v].tag = unused;
    return g;
  }
  else {
      //coloring for debuging purposes
    Vertex v_parent = add_vertex(g);
    g[v_parent].index = cat.index;
    g[v_parent].revid = cat.revid;
    g[v_parent].title = cat.title;
    g[v_parent].tag = parent;
    if(depth == max_depth) {
        g[v_parent].color = {.9, .1, .9, .8};
        g[v_parent].tag = root;
    }

    depth--;
    std::vector<Category> children = _wikidb.getCategoryChildren(cat.index);
    while(children.size() >= 1) {
        build(g, children.back(), depth);
        VertexIt vi, vi_end;
        for(boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
            Vertex v_child = *vi;
            if(get(&CatProp::tag, g, v_child) == parent
                and g[v_parent].index != g[v_child].index) {

                g[v_child].color = {.9, .9, .0, .9};
                Category cat = _wikidb.getCategory(g[v_child].index);
                for(auto parent : cat.getParents()) {
                    // build edges to parent nodes
                    if(g[v_parent].index == parent.index) {
                        EdgePair ep0 = add_edge(v_child, v_parent, g);
                        std::array<float, 4> col = {.0, .9, .9, .9};
                        put(&EdgeProp::color, g, ep0.first, col);
                        g[v_child].tag = used;
                    }
                }
            }
            //build edges to leaves
            if(get(&CatProp::tag, g, v_child) == unused) {
                EdgePair ep0 = add_edge(v_parent, v_child, g);
                g[v_child].tag = used;
            }
        }
       children.pop_back();
    }
    return g;
  }
}


boost::property_map<Graph, Point CatProp::*>::type
Model::layout_circular(double const& radius)
{
    auto pos_map = get(&CatProp::position, _graph);
    boost::circle_graph_layout(_graph, pos_map, radius);

    return pos_map;
}

PosMap
Model::layout_FR()
{
    using Topology = boost::circle_topology<boost::mt19937>;
    using Position = Topology::point_type;

    boost::property_map<Graph, Point CatProp::*>::type
    pos_map = get(&CatProp::position, _graph);
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
    );
    return pos_map;
}


boost::property_map<Graph, Point CatProp::*>::type
Model::layout_random()
{
    auto pos_map = get(&CatProp::position, _graph);
    boost::square_topology<boost::mt19937> topo;
    boost::random_graph_layout(_graph, pos_map, topo);

    return pos_map;
}

void
Model::write_layout(boost::property_map<Graph, Point CatProp::*>::type pos_map)
{
    typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
        boost::graph_traits<Graph>::vertex_descriptor vertex = *vi;
        _graph[vertex].position[0] = pos_map[vertex][0];
        _graph[vertex].position[1] = pos_map[vertex][1];
    }
}

bool
Model::dump_graph(Graph& g, std::string filename) const
{
  // Dump graph in dot format for debugging
  auto name_map = boost::get(&vta::CatProp::title, g);
  boost::default_writer dw;
  std::ofstream file(filename);
  if(file.is_open()) {
    boost::write_graphviz(file, g,
        boost::make_label_writer(name_map),
        dw, dw, get(&vta::CatProp::index, g));
    return  true;
  }
  else {return false;}
}

bool
Model::find(std::string const& cat, Category& category) const
{
    //TODO functions searches also index and revid
    auto is_digit = [] (std::string s) {return std::find_if(s.begin(), s.end(), [] (char c) {return !std::isdigit(c); }) == s.end(); };
    if(is_digit(cat)) {
        int number = std::stoi(cat);
        if(_wikidb.categoryExists(number)){
            category = _wikidb.getCategoryByRevid(number);
            return true;
        } else
            return false;
    } else {
        if(_wikidb.categoryExists(cat)) {
            category = _wikidb.getCategoryByName(cat);
            return true;
        } else
            return false;
    }
}


std::vector<std::pair<glm::vec3, std::array<float, 4> > >
Model::get_nodes() const
{
  using NodeIt = boost::graph_traits<Graph>::vertex_iterator;
  using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  using Pair = std::pair< glm::vec3,
                          std::array<float, 4> >;
  NodeIt vi, vi_end;
  std::vector<Pair> node_vec;

  for(boost::tie(vi, vi_end) = vertices(_graph); vi != vi_end; ++vi) {
    Vertex vertex = *vi;
    glm::vec3 pos;
    pos[0] = (float)_graph[vertex].position[0];
    pos[1] = (float)_graph[vertex].position[1];
    pos[2] = 0.0f;
    std::array<float, 4> color = _graph[vertex].color;
    Pair node_prop(pos, color);
      node_vec.push_back(node_prop);
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
        auto edge_color = _graph[edge].color;
        Tuple tuple(source_pos, target_pos, edge_color);
        edge_vec.push_back(tuple);
    }
    return edge_vec;
}


Category
Model::posToCat(glm::vec3 target)
{
    auto pos_map = get(&vta::CatProp::position, _graph);
    int cat_index(1);
    Category cat;
    for(auto vp = boost::vertices(_graph); vp.first != vp.second; ++vp.first) {
        auto vertex_iter = vp.first;
        auto point = get(pos_map, *vertex_iter);
        glm::vec3 source(point[0], point[1], 0.0f);
        auto distance = glm::distance(target, source);
        if(distance < 0.011f) {
            std::cout << "mouse Position: (" <<  glm::to_string(target) << ")\n";
            std::cout << "Vertex Position: (" << glm::to_string(source)<< ")\n";
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
