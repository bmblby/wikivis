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
   _cat2art.clear();
   _categories.clear();
   buildDFS(g, root, depth);
   _graph = g;
   std::cout << "number of vertices: " << num_vertices(_graph) << "\n";
}

Graph
Model::buildDFS(Graph& g, Category const& cat, size_t depth)
{
    Vertex v_parent;
    if(depth == _max_depth) {
        //coloring for debuging purposes
        v_parent = add_vertex(g);
        g[v_parent].index = cat.index;
        g[v_parent].revid = cat.revid;
        g[v_parent].title = cat.title;

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
            g[pair.first].color = YELLOW;
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
    _max_depth = depth;
    _cat2art.clear();
    _categories.clear();
    _simM.clear();
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
            v = add_vertex(g);
            g[v].index = cat.index;
            g[v].revid = cat.revid;
            g[v].title = cat.title;
            g[v].level = 0;

            // fill _art, _cat and _simM
            auto art_size = fill_data(cat, v);
            g[v].num_articles = art_size;
            g[v].num_categories = _wikidb.getChildrenCatID(cat.index).size();
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

std::pair<bool, Vertex>
Model::in_graph(Graph& g, uint32_t index) const
{
    for(auto vp = vertices(g); vp.first != vp.second; ++vp.first) {
        if(g[*vp.first].index == index)
            return std::make_pair(true, *vp.first);
    }
    Vertex v;
    return std::make_pair(false, v);
}

void
Model::expandCat(Category const& cat)
{
    // write fucntion to exapnd clicked category
    // only expand leave categories

    //search vertex of cat in graph
    auto revid_map = get(&vta::CatProp::revid, _graph);
    for(auto vp = boost::vertices(_graph); vp.first != vp.second; ++vp.first) {
        auto vertex = *vp.first;
        auto revid = get(revid_map, vertex);
        if(revid == cat.revid and out_degree(vertex, _graph) == 0) {
            std::cout << "title: " << _graph[vertex].title
                << " out degree: "  << out_degree(vertex, _graph) << std::endl;

            float radius = _r;
            size_t i = 0;
            int lvl = _graph[vertex].level + 1;
            float dist = radius/_max_depth;
            float last_cat_angle = 0.0f;
            float deg_prev_cat = 0.0f;

            double l_lim = std::min(_graph[vertex].l_bis_lim, _graph[vertex].l_tan_lim);
            double r_lim = std::max(_graph[vertex].r_bis_lim, _graph[vertex].r_tan_lim);
            radius += dist * (lvl - 1);
            // double angle_space = (l_lim - r_lim)/out_degree(vertex, _graph);
            double angle_space = (l_lim - r_lim);///_graph[vertex].num_categories;


            // add vertex for every children to graph
            auto children = _wikidb.getCategoryChildren(cat.index);
            std::cout << "num vertices: " << num_vertices(_graph) << std::endl;
            for(auto const& child : children) {
                auto pair = add_cat(_graph, child, vertex);
                _graph[pair.first].pos[0] += radius * cos(angle_space * i + r_lim);
                _graph[pair.first].pos[1] += radius * sin(angle_space * i + r_lim);
                std::cout << "radius: " << radius
                            << " angle_space: " << angle_space
                            << " l_lim: " << l_lim
                            << " r_lim: " << r_lim << std::endl;
                std::cout << "position: " << _graph[pair.first].pos[0] << " : "
                << _graph[pair.first].pos[1] << std::endl;
                ++i;
            }
            break;
        }
    }
    std::cout << cat.title << std::endl;
    std::cout << "num vertices: " << num_vertices(_graph) << std::endl;
}

void
Model::expand_leaves(int depth)
{
    do {
        for(auto vp = vertices(_graph); vp.first != vp.second; ++vp.first) {
            if(out_degree(*vp.first, _graph) == 0) {
                auto index = _graph[*vp.first].index;
                auto children = _wikidb.getCategoryChildren(index);
                // std::cout << "num vertices: " << num_vertices(_graph) << std::endl;
                for(auto const& child : children) {
                    auto pair = add_cat(_graph, child, *vp.first);
                }
            }
        }
        depth--;
    } while (depth >= 1);

}

std::pair<Vertex, EdgePair>
Model::add_cat(Graph& g, Category const& cat,
                        Vertex const& parent, std::array<float, 4> color)
{
    Vertex v = add_vertex(g);
    g[v].index = cat.index;
    g[v].revid = cat.revid;
    g[v].title = cat.title;
    g[v].level = g[parent].level + 1;
    g[v].pos[0] = 0;
    g[v].pos[1] = 0;
    g[v].color = color;
    EdgePair ep0 = add_edge(parent, v, g);

    //fill _art, _cat and _simM
    auto art_size = fill_data(cat, v);
    g[v].num_articles = art_size;
    g[v].num_categories = _wikidb.getChildrenCatID(cat.index).size();
    std::pair<Vertex, EdgePair> p(v, ep0);
    return p;
}

boost::property_map<Graph, Point CatProp::*>::type
Model::layout_circular(double const& radius)
{
    auto pos_map = get(&CatProp::pos, _graph);
    boost::circle_graph_layout(_graph, pos_map, radius);
    return pos_map;
}

PosMap
Model::layout_FR()
{
    using Topology = boost::circle_topology<boost::mt19937>;
    using Position = Topology::point_type;

    auto pos_map = get(&CatProp::pos, _graph);
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
    auto pos_map = get(&CatProp::pos, _graph);
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
        _graph[vertex].pos[0] = pos_map[vertex][0];
        _graph[vertex].pos[1] = pos_map[vertex][1];
        std::cout << pos_map[vertex][0] << " "
        << pos_map[vertex][1] << std::endl;
    }
}

struct layout_visitor : public boost::default_bfs_visitor
{
    template<typename PosMap>
    layout_visitor(size_t width, size_t height, size_t depth, float radius, PosMap& pos_map)
    :_w(width), _h(height), _depth(depth), _pmap(pos_map), _r(radius){}

    template<typename Vertex, typename Graph>
    void print(Vertex child, Graph g, double arc_angle, double angle_space, double dist)
    {
        std::cout << g[child].title << " bisector limits: "
        << g[child].r_bis_lim << ":"
        << g[child].l_bis_lim << " tangent limits: "
        << g[child].r_tan_lim << ":"
        << g[child].l_tan_lim << " deg prev cat: "
        << g[child].deg_prev_cat << " angle: "
        << g[child].angle << " angle_space: "
        << angle_space << " arc_angle: "
        << arc_angle << " dist: "
        << dist
        << std::endl;
    }

    template<typename Vertex, typename Graph>
    float set_boundaries(Vertex v, Graph& g, float dist, int level, int index, double angle_space, double r_lim)
    {
        g[v].angle = angle_space * index + r_lim;
        g[v].deg_prev_cat = g[v].angle - _last_cat_angle;
        g[v].r_bis_lim = g[v].angle - g[v].deg_prev_cat/2;
        g[v].l_bis_lim = g[v].angle + g[v].deg_prev_cat/2;

        double arc_angle = 4 * asin(dist * level /(dist * (level + 1)));
        g[v].l_tan_lim = g[v].angle + (arc_angle/2);
        g[v].r_tan_lim = g[v].angle - (arc_angle/2);
        // _last_cat_angle = g[v].angle;

        // print(v, g, arc_angle, angle_space, dist);
        return  g[v].angle;
    }

    template<typename Vertex, typename Graph>
    void reset_first(Vertex v, Graph& g, float dist, int level, double deg_to_first)
    {
        g[v].r_bis_lim = g[v].angle - deg_to_first/2;
        g[v].l_bis_lim = g[v].angle + deg_to_first/2;
        double arc_angle = 4 * asin(dist * level /(dist * (level + 1)));
        g[v].l_tan_lim = g[v].angle + (arc_angle/2);
        g[v].r_tan_lim = g[v].angle - (arc_angle/2);
    }

    template<typename Vertex, typename Graph>
    void equal_radial(Vertex v, Graph& g)
    {
        int index = 0;
        float radius = _r;
        int level = g[v].level + 1;
        double dist = radius/ _depth;
        double deg_to_first;

        if(g[v].level == 0) {
            g[v].pos[0] = 0;
            g[v].pos[1] = 0;
            double angle_space = 2*M_PI / out_degree(v, g);

            auto ep = boost::out_edges(v, g);
            Vertex first_cat = boost::target(*ep.first, g);
            for(;ep.first != ep.second; ep.first++) {
                auto child = boost::target(*ep.first, g);

                //set position and color of nodes on level 1
                g[child].pos[0] += radius * cos(index * angle_space);
                g[child].pos[1] += radius * sin(index * angle_space);
                g[child].color = YELLOW;

                //calculate bisector and tangent limits
                _last_cat_angle = set_boundaries(child, g, dist, level, index, angle_space, 0);
                deg_to_first = g[child].deg_prev_cat;
                index++;
            }
            reset_first(first_cat, g, dist, level, deg_to_first);
            _last_cat_angle = 0;
        }
        else if (g[v].level >= 1) {
            _last_cat_angle  = 0.0f;
            double l_lim = std::min(g[v].l_bis_lim, g[v].l_tan_lim);
            double r_lim = std::max(g[v].r_bis_lim, g[v].r_tan_lim);
            radius += dist * g[v].level;
            double angle_space = (l_lim - r_lim)/out_degree(v, g);

            auto ep = boost::out_edges(v, g);
            Vertex first_cat = boost::target(*ep.first, g);
            for(; ep.first != ep.second; ep.first++) {
                auto child = boost::target(*ep.first, g);
                g[child].pos[0] += radius * cos(angle_space * index + r_lim);
                g[child].pos[1] += radius * sin(angle_space * index + r_lim);
                // caluclate limits if category has children
                if(boost::out_degree(v, g) > 0){
                    _last_cat_angle = set_boundaries(child, g, dist, level, index, angle_space, r_lim);
                }
                deg_to_first = g[child].deg_prev_cat;
                index++;
            }
            reset_first(first_cat, g, dist, level, deg_to_first);
        }
    }

    template<typename Vertex, typename Graph>
    void discover_vertex(Vertex v, Graph& g)
    {
        equal_radial(v, g);
    }

    //member
    size_t _w;
    size_t _h;
    double _last_cat_angle = 0;
    double _deg_prev_cat = 0;
    size_t _depth;
    float _r;
    PosMap& _pmap;
};

glm::vec3
Model::pol2cart(float r, float phi) {
    glm::vec3 v;
    v.x = r * cos(phi);
    v.y = r * sin(phi);
    return v;
}

std::pair<float, float>
Model::cart2pol(glm::vec3 p) {
    std::pair<float, float> pol;
    pol.first = hypot(p.x, p.y);
    pol.second = atan2(p.x, p.y);
    return pol;
}

float
Model::tau(float rho) {
    return 2*acos(rho / (rho + _r) );
}

void
Model::free_tree(Vertex v, float rho, float a1, float a2)
{
    float s;
    float alpha;
    auto cart = pol2cart(rho, (a1 + a2) );
    _graph[v].pos[0] = cart.x;
    _graph[v].pos[1] = cart.y;

    if(out_degree(v, _graph) == 0)
        return;
    if(tau(rho) < a2 -a1) {
        s = tau(rho) / _graph[v].wideness;
        alpha = (a1 + a2 - tau(rho))/2;
    }
    else {
        s = (a2 - a1)/_graph[v].wideness;
        alpha = a1;
    }
    for(auto ep = out_edges(v, _graph); ep.first != ep.second; ++ep.first) {
        auto child = target(*ep.first, _graph);
        free_tree(child, rho + _r, alpha, alpha + s * _graph[child].wideness);
        alpha = alpha + s * _graph[child].wideness;
    }
}

void
Model::threshold(float value)
{
    uint32_t sim_val = value *1000;
    // std::cout << "current slider value: " << sim_val << std::endl;

    //reset all categories
    for(auto cat : _cat2art)
        _graph[cat.first].color = BLUE_0;
    std::vector<std::pair<uint32_t, SimPair>> pair_vec;
    std::set<uint32_t> target;

    //find articles with over threshold
    for(auto it = _simM.begin(); it != _simM.end(); ++it) {
        for(auto sp : it->second) {
            if(sp.getSim() >= sim_val) {
                if(_simM.find(sp.getIndex()) != _simM.end()) {
                    pair_vec.push_back(std::make_pair(it->first, sp));
                    target.insert(sp.getIndex());
                    target.insert(it->first);
                }
            }
        }
    }
    //color cat with article in set
    for(auto cat : _cat2art) {
        if(target.find(cat.second) != target.end())
            _graph[cat.first].color = YELLOW;
    }
    _dirty = true;
}

void
Model::focus_cat(uint32_t index, float threshold)
{
    uint32_t sim_val = threshold *1000;
    std::cout << "current slider value: " << sim_val << std::endl;
    //reset all categories
    for(auto cat : _cat2art)
        _graph[cat.first].color = BLUE_0;
    std::vector<std::pair<uint32_t, SimPair>> pair_vec;
    std::set<uint32_t> source;
    std::set<uint32_t> target;

    // find articles inside cat
    auto p = in_graph(_graph, index);
    auto articles = _cat2art.equal_range(p.second);
    for(auto it = articles.first; it != articles.second; ++it) {
        source.insert(it->second);
    }
    std::cout << "source size: " << source.size() << std::endl;

    // find target_articles in simMatrix
    for(auto it = _simM.begin(); it != _simM.end(); ++it) {
        for(auto sp : it->second) {
            if(sp.getSim() >= sim_val) {
                if(source.find(sp.getIndex()) != source.end()) {
                    pair_vec.push_back(std::make_pair(it->first, sp));
                    target.insert(sp.getIndex());
                }
            }
        }
    }
    std::cout << "target size: " << target.size() << std::endl;

    //color categories with target_articles
    for(auto i : target) {
        if(_art2cat.find(i) != _art2cat.end()) {
            auto cat = _art2cat.find(i)->second;
            std::cout << _graph[cat].title << std::endl;
            _graph[cat].color = YELLOW;
        }
    }
    _dirty = true;
}

struct width_visitor : public boost::default_dfs_visitor
{
    width_visitor(){}
    template<typename Vertex, typename Graph>
    void discover_vertex(Vertex v, Graph& g)
    {
        if(out_degree(v, g) == 0)
            g[v].wideness = 1;
    }

    template<typename Vertex, typename Graph>
    void finish_vertex(Vertex v, Graph& g)
    {
        for(auto ep = out_edges(v, g); ep.first != ep.second; ++ep.first) {
            Vertex child = target(*ep.first, g);
            g[v].wideness += g[child].wideness;
        }
    }
};

PosMap
Model::layout(Category const& cat, size_t width, size_t height, size_t depth, float radius)
{
    _r = radius;
    _max_depth = depth;
    auto p = in_graph(_graph, cat);
    if(p.first) {
        Vertex start = p.second;
        PosMap pos_map;
        width_visitor set_width;
        depth_first_search(_graph, visitor(set_width));

        free_tree(p.second, _graph[p.second].pos[0], _graph[p.second].pos[1], 2*M_PI);
    }
    return get(&vta::CatProp::pos, _graph);

    // debug
    // for(auto vp = vertices(_graph); vp.first != vp.second; vp.first++) {
    //     std::cout << _graph[*vp.first].title
    //     << " x: " << _graph[*vp.first].pos[0]
    //     << " y: " << _graph[*vp.first].pos[1] << std::endl;
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
    pos[0] = (float)_graph[vertex].pos[0];
    pos[1] = (float)_graph[vertex].pos[1];
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
        source_pos[0] = _graph[source].pos[0];
        source_pos[1] = _graph[source].pos[1];
        source_pos[2] = 0.0f;
        glm::vec3 target_pos;
        target_pos[0] = _graph[target].pos[0];
        target_pos[1] = _graph[target].pos[1];
        target_pos[2] = 0.0f;
        auto edge_color = _graph[edge].color;
        Tuple tuple(source_pos, target_pos, edge_color);
        edge_vec.push_back(tuple);
    }
    return edge_vec;
}

uint32_t
Model::fill_data(Category const& cat, Vertex v)
{
    _categories.insert(cat.index);
    std::vector<uint32_t> articles = _wikidb.getChildrenArtID(cat.index);
    for(auto i : articles) {
        _simM[i] = _wikidb.getComparisons(i);
        _cat2art.insert(std::make_pair(v, i));
        _art2cat.insert(std::make_pair(i, v));
    }
    return articles.size();
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
    auto pos_map = get(&vta::CatProp::pos, _graph);
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
template <class TitleMap, class IndexMap, class RevidMap, class PositionMap, class LevelMap, class WidthMap, class LBisMap, class RBisMap, class LTanMap, class RTanMap, class PrevDegCatMap>
class vertex_writer
{
    public:
        vertex_writer(TitleMap tm, IndexMap im, RevidMap rm, PositionMap pm, LevelMap lm, WidthMap wm,  LBisMap lbm,  RBisMap rbm,  LTanMap ltm,  RTanMap rtm, PrevDegCatMap pdcm):
        _tm(tm), _im(im), _rm(rm), _pm(pm), _level(lm), _wm(wm), _lbm(lbm), _rbm(rbm), _ltm(ltm),
        _rtm(rtm), _pdcm(pdcm)
        {}

        void operator()(std::ostream& out, Vertex const& v) const {
            auto point = get(_pm, v);
            out << "[index=\"" << _im(v) << "\", "
            << "revid=\"" << _rm(v) << "\", "
            << "title=\"" << _tm(v) << "\", "
            << "position=\"" << point[0] << "," << point[1] << "\", "
            << "level=\"" << _level(v) << "\", "
            << "width=\"" << _wm(v) << "\", "
            << "lbis=\"" << _lbm(v) << "\", "
            << "rbis=\"" << _rbm(v) << "\", "
            << "ltan=\"" << _ltm(v) << "\", "
            << "rtan=\"" << _rtm(v) << "\", "
            << "pdc=\"" << _pdcm(v) << "\"]";
        }
    private:
        TitleMap _tm;
        IndexMap _im;
        RevidMap _rm;
        PositionMap _pm;
        LevelMap _level;
        WidthMap _wm;
        LBisMap _lbm;
        RBisMap _rbm;
        LTanMap _ltm;
        RTanMap _rtm;
        PrevDegCatMap _pdcm;
};

template <class TitleMap, class IndexMap, class RevidMap, class PositionMap, class LevelMap, class WidthMap, class LBisMap, class RBisMap, class LTanMap, class RTanMap, class PrevDegCatMap>
inline vertex_writer<TitleMap, IndexMap, RevidMap, PositionMap, LevelMap, WidthMap, LBisMap,  RBisMap,  LTanMap,  RTanMap, PrevDegCatMap>
make_vertex_writer(TitleMap t, IndexMap i, RevidMap r, PositionMap p, LevelMap lm, WidthMap wm,  LBisMap lbm,  RBisMap rbm,  LTanMap ltm,  RTanMap rtm, PrevDegCatMap pdcm) {
    return vertex_writer<TitleMap, IndexMap, RevidMap, PositionMap, LevelMap, WidthMap,  LBisMap,  RBisMap,  LTanMap,  RTanMap, PrevDegCatMap>(t, i, r, p, lm, wm, lbm, rbm, ltm, rtm, pdcm);
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
Model::dump_graph(std::string filename) const
{
  //TODO add edge writer with position an name for convenience
  auto t_map = boost::get(&vta::CatProp::title, _graph);
  auto i_map = boost::get(&vta::CatProp::index, _graph);
  auto r_map = boost::get(&vta::CatProp::revid, _graph);
  auto p_map = boost::get(&vta::CatProp::pos, _graph);

  auto level_map = boost::get(&vta::CatProp::level, _graph);
  auto width_map = boost::get(&vta::CatProp::wideness, _graph);
  auto l_bis_map = boost::get(&vta::CatProp::l_bis_lim, _graph);
  auto r_bis_map = boost::get(&vta::CatProp::r_bis_lim, _graph);
  auto l_tan_map = boost::get(&vta::CatProp::l_tan_lim, _graph);
  auto r_tan_map = boost::get(&vta::CatProp::r_tan_lim, _graph);
  auto deg_prev_map = boost::get(&vta::CatProp::deg_prev_cat, _graph);

  boost::default_writer dw;
  std::ofstream file(filename);
  if(file.is_open()) {
    boost::write_graphviz(file, _graph,
    make_vertex_writer(t_map, i_map, r_map, p_map, level_map, width_map, l_bis_map, r_bis_map, l_tan_map, r_tan_map, deg_prev_map),
    //   make_edge_writer(t_map));
    dw);
    return  true;
  }
  else
    return false;
}

} // Namespace vta
