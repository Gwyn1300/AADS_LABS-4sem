#ifndef FUNCTION_H
#define FUNCTION_H

#include "Graph.h"
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <vector>

template<typename Vertex, typename Distance>
Vertex store(const Graph<Vertex, Distance>& graph) {
    // Получаем все вершины графа
    std::vector<Vertex> vertices = graph.vertices();
    
    // Проверка на пустой граф
    if (vertices.empty()) {
        throw std::runtime_error("Graph is empty. Cannot find warehouse location.");
    }
    
    // Если в графе одна вершина, она и будет складом
    if (vertices.size() == 1) {
        return vertices[0];
    }
    
    // Для хранения эксцентриситета каждой вершины
    std::unordered_map<Vertex, Distance> eccentricity;
    const Distance INF = std::numeric_limits<Distance>::max();
    
    // Для каждой вершины вычисляем максимальное расстояние до других
    for (const auto& source : vertices) {
        Distance max_dist = 0;
        bool has_paths = false;
        
        for (const auto& target : vertices) {
            if (source == target) continue;
            
            // Находим кратчайший путь
            auto path = graph.shortest_path(source, target);
            
            if (!path.empty()) {
                has_paths = true;
                Distance total = 0;
                for (const auto& edge : path) {
                    total += edge.distance.value();
                }
                if (total > max_dist) {
                    max_dist = total;
                }
            }
        }
        
        // Если вершина не связана с другими (недостижима), пропускаем её
        if (!has_paths) {
            eccentricity[source] = INF;
        } else {
            eccentricity[source] = max_dist;
        }
    }
    
    // Находим вершину с минимальным эксцентриситетом
    Vertex best = vertices[0];
    Distance min_eccentricity = eccentricity[best];
    
    for (const auto& vertex : vertices) {
        if (eccentricity[vertex] < min_eccentricity) {
            min_eccentricity = eccentricity[vertex];
            best = vertex;
        }
    }
    
    return best;
}

#endif