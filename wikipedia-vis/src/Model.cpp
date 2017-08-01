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
        std::cout << pos_map[vertex][0] << " "
        << pos_map[vertex][1] << std::endl;
    }
}

struct layout_visitor : public boost::default_bfs_visitor
{
    template<typename PosMap>
    layout_visitor(size_t width, size_t height, size_t depth, PosMap& pos_map)
    :_w(width), _h(height), _depth(depth), _pmap(pos_map){}

    template<typename Vertex, typename Graph>
    void set_level(Vertex v, Graph& g) {
        if(in_degree(v, g) != 0) {
            auto ep = boost::in_edges(v, g);
            auto parent = boost::source(*ep.first, g);
            int test = g[parent].level;
            g[v].level = test + 1;
        }
    }

    template<typename Vertex, typename Graph>
    void discover_vertex(Vertex v, Graph& g)
    {
        double radius = .2;
        double dist = radius/ _depth;
        set_level(v, g);
        if(g[v].level == 0) {
            g[v].position[0] = 0;
            g[v].position[1] = 0;
            double angle_space = 2*M_PI / g[v].num_categories;

            // set position for level 1 children;
            int index = 0;
            for(auto ep = boost::out_edges(v, g); ep.first != ep.second; ep.first++) {
                auto child = boost::target(*ep.first, g);
                g[child].position[0] += radius * cos(index * angle_space);
                g[child].position[1] += radius * sin(index * angle_space);
                g[child].color = YELLOW_SOFT;

                //calculate bisector and tangent limits
                g[child].angle = angle_space * index;
                g[child].deg_prev_cat = g[child].angle - last_cat_angle;
                g[child].r_bis_lim = g[child].angle - deg_prev_cat/2;
                g[child].l_bis_lim = g[child].angle + deg_prev_cat/2;
                double arc_angle = 4 * asin(dist * _depth /dist * (_depth + 1));
                g[child].l_tan_lim = g[child].angle + (arc_angle/2);
                g[child].r_tan_lim = g[child].angle - (arc_angle/2);

                last_cat_angle = g[child].angle;
                deg_prev_cat = g[child].deg_prev_cat;
                index++;
            }
        }
        else if (g[v].level >= 1) {
            std::cout << " parent: " << g[v].title << std::endl;
            double l_lim = std::min(g[v].l_bis_lim, g[v].l_tan_lim);
            double r_lim = std::max(g[v].r_bis_lim, g[v].r_tan_lim);
            radius += dist * g[v].level;
            double angle_space = (l_lim - r_lim)/ g[v].num_categories;

            int index = 0;
            for(auto ep = boost::out_edges(v, g); ep.first != ep.second; ep.first++) {
                auto child = boost::target(*ep.first, g);
                std::cout << "processing child: " << g[child].title << std::endl;

                g[child].position[0] += radius * cos(angle_space * index + r_lim);
                g[child].position[1] += radius * sin(angle_space * index + r_lim);
                g[child].angle = angle_space * index + r_lim;
                if(g[child].num_categories > 0){
                    g[child].angle = angle_space * index;
                    g[child].deg_prev_cat = g[child].angle - last_cat_angle;
                    g[child].r_bis_lim = g[child].angle - deg_prev_cat/2;
                    g[child].l_bis_lim = g[child].angle + deg_prev_cat/2;
                    double arc_angle = 4 * asin(dist * _depth /dist * (_depth + 1));
                    g[child].l_tan_lim = g[child].angle + (arc_angle/2);
                    g[child].r_tan_lim = g[child].angle - (arc_angle/2);

                    last_cat_angle = g[child].angle;
                    deg_prev_cat = g[child].deg_prev_cat;
                }
                index++;
            }
        }

        // debug
        std::cout << g[v].title
        << " x: " << g[v].position[0]
        << " y: " << g[v].position[1] << std::endl;
        // std::cout << g[v].title <<  "<-title : level ->" << g[v].level << std::endl;
    }


    //member
    float last_cat_angle  = 0.0f;
    float deg_prev_cat = 0.0f;
    size_t _w;
    size_t _h;
    size_t _depth;
    size_t _index = 0;
    size_t _index1 = 0;
    PosMap& _pmap;

};

// boost::property_map<Graph, Point CatProp::*>::type
PosMap
Model::layout(Category const& cat, size_t width, size_t height, size_t depth)
{
    auto p = in_graph(_graph, cat);
    if(p.first) {
        Vertex start = p.second;
        PosMap pos_map;
        layout_visitor vis(width, height, depth, pos_map);
        breadth_first_search(_graph, start, visitor(vis));
    }
    return get(&vta::CatProp::position, _graph);

    // debug
    // for(auto vp = vertices(_graph); vp.first != vp.second; vp.first++) {
    //     std::cout << _graph[*vp.first].title
    //     << " x: " << _graph[*vp.first].position[0]
    //     << " y: " << _graph[*vp.first].position[1] << std::endl;
    //  }
}

std::vector<std::pair<glm::vec3, std::array<float, 4> > >
Model::get_nodes() const
{
  // using NodeIt = boost::graph_traits<Graph>::vertex_iterator;
  // using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
  // NodeIt vi, vi_end;
  using Pair = std::pair< glm::vec3,
                          std::array<float, 4> >;
  std::vector<Pair> node_vec;

  for(auto vp = vertices(_graph); vp.first != vp.second; ++vp.first) {
    Vertex vertex = *vp.first;
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
    // using EdgeIt = boost::graph_traits<Graph>::edge_iterator;
    // using Edge = boost::graph_traits<Graph>::edge_descriptor;
    // using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
    // EdgeIt ei, ei_end;
    using Tuple = std::tuple<const glm::vec3,
                             const glm::vec3,
                             const std::array<float, 4>>;

    std::vector<Tuple> edge_vec;
    for(auto ep = edges(_graph); ep.first != ep.second; ++ep.first) {
        Edge edge = *ep.first;
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

// Dump graph to file with graphviz
template <class TitleMap, class IndexMap, class RevidMap, class PositionMap>
class vertex_writer
{
    public:
        vertex_writer(TitleMap tm, IndexMap im, RevidMap rm, PositionMap pm):
        _tm(tm), _im(im), _rm(rm), _pm(pm)
        {}

        void operator()(std::ostream& out, Vertex const& v) const {
            auto point = get(_pm, v);
            out << "[index=\"" << _im(v) << "\", revid=\"" << _rm(v)
                << "\", title=\"" << _tm(v) << "\", "
                << "position=\"" << point[0] << "," << point[1] << "\"]";
        }
    private:
        TitleMap _tm;
        IndexMap _im;
        RevidMap _rm;
        PositionMap _pm;
};

template <class TitleMap, class IndexMap, class RevidMap, class PositionMap>
inline vertex_writer<TitleMap, IndexMap, RevidMap, PositionMap>
make_vertex_writer(TitleMap t, IndexMap i, RevidMap r, PositionMap p) {
    return vertex_writer<TitleMap, IndexMap, RevidMap, PositionMap>(t, i, r, p);
}

template <class TitleMap>
class edge_writer
{
    public:
        edge_writer(TitleMap tm)
        :_tm(tm) {}
        void operator()(std::ostream& out, Vertex const& ep) const {
            out << "\"" << _tm(ep) << "\"->\"" << _tm(ep) << "\"]";
        }
    private:
        TitleMap _tm;
};

template <class TitleMap>
inline edge_writer<TitleMap>
make_edge_writer(TitleMap t) {
    return edge_writer<TitleMap>(t);
}

bool
Model::dump_graph(Graph& g, std::string filename) const
{
  //TODO add edge writer with position an name for convenience
  auto t_map = boost::get(&vta::CatProp::title, g);
  auto i_map = boost::get(&vta::CatProp::index, g);
  auto r_map = boost::get(&vta::CatProp::revid, g);
  auto p_map = boost::get(&vta::CatProp::position, g);
  boost::default_writer dw;

  std::ofstream file(filename);
  if(file.is_open()) {
    boost::write_graphviz(file, g,
    make_vertex_writer(t_map, i_map, r_map, p_map),
    //   make_edge_writer(t_map));
    dw);
    return  true;
  }
  else
    return false;
}

} // Namespace vta
