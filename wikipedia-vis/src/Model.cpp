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

void
Model::initGraph(Category const& root, size_t depth)
{
   Graph g;
   _max_depth = depth;
   buildDFS(g, root, depth);
   _graph = g;
   std::cout << "number of vertices: " << num_vertices(_graph) << "\n";
}

Graph
Model::buildDFS(Graph& g, Category const& cat, size_t depth)
{
    enum {white, grey, black, root};

    Vertex v_parent;
    if(depth == _max_depth) {
        //coloring for debuging purposes
        v_parent = add_vertex(g);
        g[v_parent].index = cat.index;
        g[v_parent].revid = cat.revid;
        g[v_parent].title = cat.title;
        g[v_parent].color = PINK;
        g[v_parent].tag = root;

        g[v_parent].num_articles = _wikidb.getArticleChildren(cat.index).size();
        g[v_parent].num_categories = _wikidb.getArticleChildren(cat.index).size();
        _root = v_parent;
    }
    else {
    if(in_graph(g, cat).first)
        v_parent = in_graph(g, cat).second;
    }
    depth--;

    if(depth > 0) {
        std::vector<Category> children = _wikidb.getCategoryChildren(cat.index);
        while(children.size() >= 1) {
            if(in_graph(g, children.back()).first) {
                children.pop_back();
                continue;
            }
            auto pair = add_cat(g, children.back(), v_parent);
            g[pair.first].color = YELLOW_SOFT;
            buildDFS(g, children.back(), depth);
            children.pop_back();
        }
        depth++;
        return g;
    }
    else {
        std::vector<Category> children = _wikidb.getCategoryChildren(cat.index);
        while(children.size() >= 1) {
            if(in_graph(g, children.back()).first) {
                children.pop_back();
                continue;
            }
            auto pair = add_cat(g, children.back(), v_parent);
            children.pop_back();
        }
        return g;
    }
}

void
Model::initIDDFS(Category const& root, size_t depth)
{
    Graph g;
    enum {white, grey, black};
    for(size_t i = 0; i <= depth; ++i) {
        Vertex v;
        buildDLS(g, root, v, i);
    }
    _graph = g;
}

Graph
Model::buildDLS(Graph& g, Category const& cat, Vertex& v, size_t depth)
{
    if(depth == 0) {
        if(num_vertices(g) == 0) {
            enum {white, grey, black, root};
            v = add_vertex(g);
            g[v].index = cat.index;
            g[v].revid = cat.revid;
            g[v].title = cat.title;
            g[v].color = PINK;
            g[v].tag = root;

            g[v].num_articles = _wikidb.getArticleChildren(cat.index).size();
            g[v].num_categories = _wikidb.getArticleChildren(cat.index).size();
            _root = v;
        }
        else if(!in_graph(g, cat).first){
            auto pair = add_cat(g, cat, v);
        }
        return g;
    }
    if(depth > 0) {
        depth--;
        // get vertex descriptor on cat
        if(in_graph(g, cat).first)
            v = in_graph(g, cat).second;
        std::vector<Category> children = _wikidb.getCategoryChildren(cat.index);
        while(children.size() >= 1) {
            buildDLS(g, children.back(), v, depth);
            children.pop_back();
        }
        depth++;
        return g;
    }
}

std::pair<bool, Vertex>
Model::in_graph(Graph& g, Category const& cat) const
{
    for(auto vp = vertices(g); vp.first != vp.second; ++vp.first) {
        if(g[*vp.first].index == cat.index
            or g[*vp.first].revid == cat.revid
            or g[*vp.first].title == cat.title) {
                return std::make_pair(true, *vp.first);
            }
    }
    Vertex v;
    return std::make_pair(false, v);
}

void
Model::expand(Category const& cat)
{
    // write fucntion to exapnd clicked category
    // only expand leave categories

    //search vertex of cat in graph
    Vertex parent;
    auto revid_map = get(&vta::CatProp::revid, _graph);
    for(auto vp = boost::vertices(_graph); vp.first != vp.second; ++vp.first) {
        auto vertex = *vp.first;
        auto revid = get(revid_map, vertex);
        if(revid == cat.revid and out_degree(vertex, _graph) == 0) {
            std::cout << "title: " << _graph[vertex].title
                << " out degree: "  << out_degree(vertex, _graph) << std::endl;
            parent = vertex;

            auto children = _wikidb.getCategoryChildren(cat.index);
            std::cout << "number of children : " << children.size() << std::endl;

            double x_parent = _graph[parent].position[0];
            double y_parent = _graph[parent].position[1];
            std::cout << "xpos parent: " << _graph[parent].position[0] << std::endl;
            std::cout << "ypos parent: " << _graph[parent].position[1] << std::endl;
            size_t i = 0;
            float radius = 0.1f;
            double two_pi_over_n = 2 * 3.14 / children.size();

            // add vertex for every children to graph
            std::cout << "num vertices: " << num_vertices(_graph) << std::endl;
            for(auto const& child : children) {
                auto pair = add_cat(_graph, child, parent);
                // std::cout << "child title: " << _graph[vert].title << std::endl;
                _graph[pair.first].position[0] += radius * cos(i * two_pi_over_n);
                _graph[pair.first].position[1] += radius * sin(i * two_pi_over_n);
                ++i;
            }
            break;
        }
    }
    std::cout << cat.title << std::endl;
    std::cout << "num vertices: " << num_vertices(_graph) << std::endl;
}

std::pair<Vertex, EdgePair>
Model::add_cat(Graph& g, Category const& cat,
            Vertex const& parent, std::array<float, 4> color)
{
    Vertex v = add_vertex(g);
    g[v].index = cat.index;
    g[v].revid = cat.revid;
    g[v].title = cat.title;
    g[v].num_articles = _wikidb.getArticleChildren(cat.index).size();
    g[v].num_categories = _wikidb.getArticleChildren(cat.index).size();

    g[v].position[0] = g[parent].position[0];
    g[v].position[1] = g[parent].position[1];
    g[v].color = color;
    EdgePair ep0 = add_edge(parent, v, g);

    std::pair<Vertex, EdgePair> p(v, ep0);
    return p;
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

    auto pos_map = get(&CatProp::position, _graph);
    Topology topo;
    boost::random_graph_layout(_graph, pos_map, topo);
    boost::fruchterman_reingold_force_directed_layout(
        _graph, pos_map, topo
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

bool
Model::find(std::string const& cat, Category& category) const
{
    auto is_digit = [] (std::string s) {return std::find_if(s.begin(), s.end(), [] (char c) {return !std::isdigit(c); }) == s.end(); };
    if(is_digit(cat)) {
        int number = std::stoi(cat);
        if(_wikidb.categoryExistsRevid(number)){
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

bool
Model::pos2cat(glm::vec3 target, Category& cat) const
{
    auto pos_map = get(&vta::CatProp::position, _graph);
    for(auto vp = boost::vertices(_graph); vp.first != vp.second; ++vp.first) {
        auto vertex_iter = vp.first;
        auto point = get(pos_map, *vertex_iter);
        glm::vec3 source(point[0], point[1], 0.0f);
        auto distance = glm::distance(target, source);
        if(distance < 0.012f) {
            // debug
            // std::cout << "mouse Position: (" <<  glm::to_string(target) << ")\n";
            // std::cout << "Vertex Position: (" << glm::to_string(source)<< ")\n";
            // std::cout << "Distance: (" << distance << ")\n\n";
            size_t cat_index = _graph[*vertex_iter].index;
            cat = _wikidb.getCategory(cat_index);
            return true;
        }
    }
    return false;
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

} // Namespace vta
