#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <optional>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <stdexcept>
#include <cstdlib> 

template<typename Vertex, typename Distance = double>
class Graph{
public:
    struct Edge{
        Vertex from;
        Vertex to;
        std::optional<Distance> distance;
    };
private:
    std::list<std::list<Edge>> matrix;
    std::list<Vertex> vertex_order;

    size_t get_index(const Vertex& v) const {
        auto it = std::find(vertex_order.begin(), vertex_order.end(), v);
        return std::distance(vertex_order.begin(), it);
    }
    
    std::optional<Distance> get_weight(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to)) {
            return std::nullopt;
        }
        size_t from_idx = get_index(from);
        size_t to_idx = get_index(to);
        auto row_it = matrix.begin();
        std::advance(row_it, from_idx);
        auto col_it = row_it->begin();
        std::advance(col_it, to_idx);
        return col_it->distance;
    }


    std::vector<Vertex> bfs(const Vertex& start) const {
        std::vector<Vertex> result;
        
        if (!has_vertex(start)) {
            return result;
        }

        std::unordered_set<Vertex> visited;
        std::queue<Vertex> q;

        visited.insert(start);
        q.push(start);

        while (!q.empty()) {
            Vertex current = q.front();
            q.pop();

            result.push_back(current);

            for (const auto& edge : edges(current)) {
                if (edge.distance.has_value() && !visited.count(edge.to)) {
                    visited.insert(edge.to);
                    q.push(edge.to);
                }
            }
        }

        return result;
    }

    std::vector<Vertex> bfs_reverse(const Vertex& start) const {
        std::vector<Vertex> result;
        std::unordered_set<Vertex> visited;
        std::queue<Vertex> q;
        
        visited.insert(start);
        q.push(start);
        
        while (!q.empty()) {
            Vertex current = q.front();
            q.pop();
            result.push_back(current);

            for (const auto& candidate : vertex_order) {
                if (!visited.count(candidate) && has_edge(candidate, current)) {
                    visited.insert(candidate);
                    q.push(candidate);
                }
            }
        }

        return result;
    }

public:
    bool has_vertex(const Vertex& v)const{
        for (const auto& row : matrix) {
            if (row.empty()) continue;
            if (row.begin()->from == v) {
                return true;
            }
        }
        return false;
    }

    bool add_vertex(const Vertex& v){

        auto vertex_it = vertex_order.begin();
        for (auto& row : matrix) {
            Vertex existing_vertex = *vertex_it;
            row.push_back(Edge{existing_vertex, v, std::nullopt});
            ++vertex_it;
        }

        std::list<Edge> new_row;

        new_row.push_back(Edge{v, v, 0.0});

        for (const auto& existing : vertex_order) {
            new_row.push_back(Edge{v, existing, std::nullopt});
        }

        matrix.push_back(new_row);
        vertex_order.push_back(v);
        return true;
    }

    bool remove_vertex(const Vertex& v){
        auto it = std::find(vertex_order.begin(),vertex_order.end(), v);
        auto k = std::distance(vertex_order.begin(), it);
        auto row_it = matrix.begin();
        std::advance(row_it, k);
        matrix.erase(row_it);

        for (auto& row : matrix) {
            auto col_it = row.begin();
            std::advance(col_it, k);
            row.erase(col_it);
        }

        vertex_order.erase(it);

        return true;
    } 

    std::vector<Vertex> vertices()const{
        return std::vector<Vertex>(vertex_order.begin(), vertex_order.end());
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        if (!has_vertex(from)) {
            throw std::invalid_argument("Source vertex not found");
        }
        if (!has_vertex(to)) {
            throw std::invalid_argument("Target vertex not found");
        }

        size_t from_idx = get_index(from);
        size_t to_idx = get_index(to);

        auto row_it = matrix.begin();
        std::advance(row_it, from_idx);

        auto col_it = row_it->begin();
        std::advance(col_it, to_idx);

        col_it->distance = d;
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from) || !has_vertex(to)) {
            return false;
        }

        size_t from_idx = get_index(from);
        size_t to_idx = get_index(to);

        auto row_it = matrix.begin();
        std::advance(row_it, from_idx);

        auto col_it = row_it->begin();
        std::advance(col_it, to_idx);

        if (!col_it->distance.has_value()) {
            return false;
        }

        col_it->distance = std::nullopt;
        return true;
    }

    bool remove_edge(const Edge& e) {
        return remove_edge(e.from, e.to);
    }
    
    bool has_edge(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to)) {
            return false;
        }

        size_t from_idx = get_index(from);
        size_t to_idx = get_index(to);

        auto row_it = matrix.begin();
        std::advance(row_it, from_idx);

        auto col_it = row_it->begin();
        std::advance(col_it, to_idx);

        return col_it->distance.has_value();
    }

    bool has_edge(const Edge& e) const {
        if (!has_vertex(e.from) || !has_vertex(e.to)) {
            return false;
        }

        size_t from_idx = get_index(e.from);
        size_t to_idx = get_index(e.to);

        auto row_it = matrix.begin();
        std::advance(row_it, from_idx);

        auto col_it = row_it->begin();
        std::advance(col_it, to_idx);

        return col_it->distance.has_value() && col_it->distance.value() == e.distance;
    }

    std::vector<Edge> edges(const Vertex& vertex) const {
        std::vector<Edge> result;
        
        if (!has_vertex(vertex)) {
            return result;  
        }

        size_t idx = get_index(vertex);
        auto row_it = matrix.begin();
        std::advance(row_it, idx);

        for (const auto& edge : *row_it) {
            if (edge.distance.has_value()) { 
                result.push_back(edge);
            }
        }

        return result;
    }
   
    size_t order() const {
        return matrix.size();
    }
    
    size_t degree(const Vertex& v) const {
        if (!has_vertex(v)) {
            return 0;  
        }

        size_t idx = get_index(v);
        auto row_it = matrix.begin();
        std::advance(row_it, idx);

        size_t deg = 0;
        for (const auto& edge : *row_it) {
            if (edge.distance.has_value()) {
                deg++;
            }
        }

        return deg;
    }

    bool is_connected() const {
        if (order() == 0) return true;
        
        Vertex start = vertex_order.front();
        
        auto from_start = bfs(start);
        if (from_start.size() != order()) return false;
        
        auto to_start = bfs_reverse(start);  
        return to_start.size() == order();
    }    
    
    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to)) {
            return {};  
        }
        if (from == to) {
            return {};  
        }

        std::unordered_map<Vertex, Distance> dist;      
        std::unordered_map<Vertex, Vertex> previous;    
        std::unordered_set<Vertex> unvisited;           

        const Distance INF = std::numeric_limits<Distance>::max();
        for (const auto& v : vertex_order) {
            dist[v] = INF;
            unvisited.insert(v);
        }
        dist[from] = 0;

        while (!unvisited.empty()) {
            Vertex current = *std::min_element(unvisited.begin(), unvisited.end(),
                [&](const Vertex& a, const Vertex& b) {
                    return dist[a] < dist[b];
                });
            
            if (dist[current] == INF) {
                break;
            }

            if (current == to) {
                break;
            }

            unvisited.erase(current);

            for (const auto& edge : edges(current)) {
                if (!edge.distance.has_value()) continue;

                Vertex neighbor = edge.to;
                Distance weight = edge.distance.value();
                Distance new_dist = dist[current] + weight;

                if (new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                    previous[neighbor] = current;
                }
            }
        }

        if (dist[to] == INF) {
            return {};  
        }

        std::vector<Edge> path;
        Vertex current = to;

        while (current != from) {
            Vertex prev = previous[current];
            for (const auto& edge : edges(prev)) {
                if (edge.to == current && edge.distance.has_value()) {
                    path.push_back(edge);
                    break;
                }
            }
            current = prev;
        }

        std::reverse(path.begin(), path.end());

        return path;
    } 
    std::vector<Vertex>  walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action)const{    
        std::vector<Vertex> order = bfs(start_vertex);
        
        for (const auto& vertex : order) {
            action(vertex);
        }

        return order;
    }
      
    void print() const {
        if (order() == 0) {
            std::cout << "Graph is empty" << std::endl;
            return;
        }

        std::ofstream html("graph_viz.html");

        html << R"(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Graph Visualization</title>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/vis-network/9.1.2/dist/vis-network.min.js"></script>
        <style>
            * {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
            }
            body {
                font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                min-height: 100vh;
                padding: 20px;
            }
            .container {
                max-width: 1400px;
                margin: 0 auto;
                background: white;
                border-radius: 20px;
                box-shadow: 0 20px 60px rgba(0,0,0,0.3);
                overflow: hidden;
            }
            .header {
                background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                color: white;
                padding: 20px;
                text-align: center;
            }
            .header h1 {
                font-size: 28px;
                margin-bottom: 5px;
            }
            .header p {
                opacity: 0.9;
                font-size: 14px;
            }
            .stats {
                display: flex;
                justify-content: center;
                gap: 30px;
                padding: 15px;
                background: #f8f9fa;
                border-bottom: 1px solid #e0e0e0;
            }
            .stat-card {
                text-align: center;
            }
            .stat-number {
                font-size: 24px;
                font-weight: bold;
                color: #667eea;
            }
            .stat-label {
                font-size: 12px;
                color: #666;
                margin-top: 5px;
            }
            #mynetwork {
                width: 100%;
                height: 600px;
                background: #fafafa;
            }
            .legend {
                padding: 15px 20px;
                background: #f8f9fa;
                border-top: 1px solid #e0e0e0;
                display: flex;
                flex-wrap: wrap;
                gap: 20px;
                font-size: 12px;
            }
            .legend-item {
                display: flex;
                align-items: center;
                gap: 8px;
            }
            .legend-color {
                width: 20px;
                height: 20px;
                border-radius: 50%;
            }
            .legend-arrow {
                width: 30px;
                height: 2px;
                background: #848484;
                position: relative;
            }
            .legend-arrow::after {
                content: "→";
                position: absolute;
                right: -12px;
                top: -8px;
                color: #848484;
                font-size: 14px;
            }
            .footer {
                text-align: center;
                padding: 15px;
                background: #f8f9fa;
                font-size: 11px;
                color: #999;
                border-top: 1px solid #e0e0e0;
            }
            @media (max-width: 768px) {
                .stats { gap: 15px; }
                .stat-number { font-size: 18px; }
            }
        </style>
    </head>
    <body>
        <div class="container">
            <div class="header">
                <h1>📊 DIRECTED GRAPH VISUALIZATION</h1>
                <p>Interactive network graph with edge weights</p>
            </div>
            <div class="stats">
                <div class="stat-card">
                    <div class="stat-number">)" << order() << R"(</div>
                    <div class="stat-label">Vertices</div>
                </div>
                <div class="stat-card">
                    <div class="stat-number">)";

        size_t edge_count = 0;
        for (const auto& from : vertex_order) {
            edge_count += degree(from);
        }
        html << edge_count << R"(</div>
                    <div class="stat-label">Edges</div>
                </div>
            </div>
            <div id="mynetwork"></div>
            <div class="legend">
                <div class="legend-item">
                    <div class="legend-color" style="background: #97C2FC;"></div>
                    <span>Vertex (Node)</span>
                </div>
                <div class="legend-item">
                    <div class="legend-arrow"></div>
                    <span>Directed Edge</span>
                </div>
                <div class="legend-item">
                    <div class="legend-color" style="background: #FF6B6B; border-radius: 2px;"></div>
                    <span>Edge weight label</span>
                </div>
            </div>
            <div class="footer">
                Drag nodes to rearrange • Hover to see details • Refresh to reset layout
            </div>
        </div>
        <script>
            var nodes = new vis.DataSet([
    )";

        int id = 0;
        std::unordered_map<Vertex, int> node_ids;
        for (const auto& v : vertex_order) {
            node_ids[v] = id;
            // Генерируем случайный цвет для каждой вершины (оттенки синего)
            html << "            {id: " << id << ", label: \"" << v << "\", shape: \"circle\", "
                 << "color: {background: \"#97C2FC\", border: \"#2B6FB6\"}, "
                 << "font: {size: 14, color: \"#333\"}, "
                 << "size: 30},\n";
            id++;
        }

        html << R"(        ]);

            var edges = new vis.DataSet([
    )";

        for (const auto& from : vertex_order) {
            for (const auto& edge : edges(from)) {
                if (edge.distance.has_value()) {
                    html << "            {from: " << node_ids[edge.from] 
                         << ", to: " << node_ids[edge.to]
                         << ", label: \"" << edge.distance.value() 
                        << "\", arrows: { from: { enabled: true, type: \"arrow\" }, to: { enabled: true, type: \"arrow\" } }, "                         << "color: { color: \"#848484\", highlight: \"#FF6B6B\" }, "
                         << "font: { align: \"middle\", size: 12, background: \"white\", strokeWidth: 1 },\n"
                         << "             smooth: { type: \"curvedCW\", roundness: 0.2 }},\n";
                }
            }
        }

        html << R"(        ]);

            var container = document.getElementById('mynetwork');
            var data = { nodes: nodes, edges: edges };
            var options = {
                nodes: {
                    shape: 'circle',
                    size: 30,
                    font: { size: 14, face: 'Segoe UI' }
                },
                edges: {
                    smooth: { type: 'curvedCW', roundness: 0.2 },
                    arrows: { to: { enabled: true, scaleFactor: 1 } },
                    font: { size: 12, align: 'middle', background: 'white', strokeWidth: 1 }
                },
                physics: {
                    enabled: true,
                    stabilization: { iterations: 100 },
                    solver: 'forceAtlas2Based',
                    forceAtlas2Based: { gravitationalConstant: -50, centralGravity: 0.01 }
                },
                interaction: {
                    hover: true,
                    tooltipDelay: 100,
                    navigationButtons: true,
                    zoomView: true,
                    dragView: true
                },
                layout: {
                    improvedLayout: true
                }
            };
            var network = new vis.Network(container, data, options);

            // Добавляем обработчик клика для отображения информации
            network.on("click", function(params) {
                if (params.nodes.length > 0) {
                    var nodeId = params.nodes[0];
                    var node = nodes.get(nodeId);
                    console.log("Clicked on: " + node.label);
                }
            });
        </script>
    </body>
    </html>
    )";

        html.close();

    #ifdef _WIN32
        system("start graph_viz.html");
    #elif __APPLE__
        system("open graph_viz.html");
    #else
        system("xdg-open graph_viz.html");
    #endif

        std::cout << "Graph visualization opened in browser (graph_viz.html)" << std::endl;
    }
};
#endif
