#include <list>
#include <optional>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <stdexcept>

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

            // Ищем все вершины, из которых есть ребро в current
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
    bool has_vertex(const Vertex& v){
        for (const auto& row : matrix) {
            if (row.empty()) continue;
            if (row.begin()->from == v) {
                return true;
            }
        }
        return false;
    }

    bool add_vertex(const Vertex& v){

        if(has_vertex(v)) return false;

        for (auto& row : matrix) {
            row.push_back(Edge{vertex_name_from_row(row), v, std::nullopt});
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

    std::vector<Vertex> vertices(){
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
        // Проверяем точное совпадение: from, to и вес
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

    //получение всех ребер, выходящих из вершины 
    std::vector<Edge> edges(const Vertex& vertex) const {
        std::vector<Edge> result;
        
        if (!has_vertex(vertex)) {
            return result;  // или бросить исключение
        }

        size_t idx = get_index(vertex);
        auto row_it = matrix.begin();
        std::advance(row_it, idx);

        for (const auto& edge : *row_it) {
            if (edge.distance.has_value()) {  // только существующие рёбра
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
            return 0;  // или бросить исключение
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
        
        // 1. Из start достижимы все?
        auto from_start = bfs(start);
        if (from_start.size() != order()) return false;
        
        // 2. В start достижимы из всех? (кто может достичь start)
        auto to_start = bfs_reverse(start);  // обход на обратных рёбрах
        return to_start.size() == order();
    }    
    
    //поиск кратчайшего пути 
    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
        // 1. Проверка существования вершин
        if (!has_vertex(from) || !has_vertex(to)) {
            return {};  // или бросить исключение
        }

        if (from == to) {
            return {};  // путь из вершины в саму себя — пустой
        }

        // 2. Инициализация
        std::unordered_map<Vertex, Distance> dist;      // кратчайшее расстояние
        std::unordered_map<Vertex, Vertex> previous;    // предыдущая вершина в пути
        std::unordered_set<Vertex> unvisited;           // непосещённые вершины

        // Устанавливаем бесконечность для всех вершин
        const Distance INF = std::numeric_limits<Distance>::max();
        for (const auto& v : vertex_order) {
            dist[v] = INF;
            unvisited.insert(v);
        }
        dist[from] = 0;

        // 3. Основной цикл Дейкстры
        while (!unvisited.empty()) {
            // Находим вершину с минимальным расстоянием среди непосещённых
            Vertex current = *std::min_element(unvisited.begin(), unvisited.end(),
                [&](const Vertex& a, const Vertex& b) {
                    return dist[a] < dist[b];
                });
            
            // Если минимальное расстояние — бесконечность, остальные недостижимы
            if (dist[current] == INF) {
                break;
            }

            // Если достигли целевой вершины, можно остановиться
            if (current == to) {
                break;
            }

            unvisited.erase(current);

            // Релаксация всех соседей
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

        // 4. Восстановление пути
        if (dist[to] == INF) {
            return {};  // путь не существует
        }

        std::vector<Edge> path;
        Vertex current = to;

        while (current != from) {
            Vertex prev = previous[current];
            // Находим ребро из prev в current с правильным весом
            for (const auto& edge : edges(prev)) {
                if (edge.to == current && edge.distance.has_value()) {
                    path.push_back(edge);
                    break;
                }
            }
            current = prev;
        }

        // Путь получился от to к from, разворачиваем
        std::reverse(path.begin(), path.end());

        return path;
    } 
    //обход 
    std::vector<Vertex>  walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action)const{    
        // Получаем порядок обхода через BFS
        std::vector<Vertex> order = bfs(start_vertex);
        
        // Применяем action к каждой вершине в порядке обхода
        for (const auto& vertex : order) {
            action(vertex);
        }

        return order;
    }
      
    void print() const {
        if (order() == 0) {
            std::cout << "┌─────────────────────────┐\n"
                      << "│       ГРАФ ПУСТ         │\n"
                      << "└─────────────────────────┘" << std::endl;
            return;
        }

        // Шапка
        std::cout << "\n  ╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "  ║                    ОРИЕНТИРОВАННЫЙ ГРАФ                      ║\n";
        std::cout << "  ╚══════════════════════════════════════════════════════════════╝\n\n";

        // Статистика
        size_t edge_count = 0;
        for (const auto& from : vertex_order) {
            edge_count += degree(from);
        }
        std::cout << "  📊 Вершин: " << order() << "  |  Рёбер: " << edge_count << "\n\n";

        // Список вершин
        std::cout << "  📍 ВЕРШИНЫ:\n  ";
        for (const auto& v : vertex_order) {
            std::cout << " [" << v << "] ";
        }
        std::cout << "\n\n";

        // Рёбра в виде списка
        std::cout << "  🔗 РЁБРА (ориентированные):\n";
        bool has_edges = false;
        for (const auto& from : vertex_order) {
            for (const auto& edge : edges(from)) {
                if (edge.distance.has_value()) {
                    has_edges = true;
                    std::cout << "     " << edge.from << " ──(" << edge.distance.value() 
                              << ")──→ " << edge.to << "\n";
                }
            }
        }
        if (!has_edges) {
            std::cout << "     (нет рёбер)\n";
        }

        // Матрица смежности (компактная)
        std::cout << "\n  📋 МАТРИЦА СМЕЖНОСТИ:\n\n";

        size_t max_len = 0;
        for (const auto& v : vertex_order) {
            std::stringstream ss;
            ss << v;
            max_len = std::max(max_len, ss.str().length());
        }
        size_t cell_width = std::max(max_len + 2, size_t(6));

        // Заголовки столбцов
        std::cout << "  " << std::string(cell_width - 2, ' ') << " │";
        for (const auto& v : vertex_order) {
            std::cout << std::setw(cell_width) << v;
        }
        std::cout << "\n  " << std::string(cell_width - 2, '─') << "─┼";
        for (size_t i = 0; i < vertex_order.size(); ++i) {
            std::cout << std::string(cell_width, '─');
        }
        std::cout << "\n";

        // Данные
        for (const auto& from : vertex_order) {
            std::cout << "  " << std::setw(cell_width - 2) << from << " │";
            for (const auto& to : vertex_order) {
                auto weight = get_weight(from, to);
                if (weight.has_value()) {
                    if (from == to && weight.value() == 0) {
                        std::cout << std::setw(cell_width) << "◉";
                    } else {
                        std::cout << std::setw(cell_width) << weight.value();
                    }
                } else {
                    std::cout << std::setw(cell_width) << "·";
                }
            }
            std::cout << "\n";
        }

        std::cout << "\n  Условные обозначения:\n";
        std::cout << "    ◉ — петля (вершина → себя)\n";
        std::cout << "    число — вес ребра\n";
        std::cout << "    · — нет ребра\n";
        std::cout << "    ─(w)─→ — ориентированное ребро с весом w\n";
        std::cout << std::endl;
    }
};
