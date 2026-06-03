#include "Graph.h"
#include "function.h"
#include <iostream>
#include <string>
#include <vector>
#include <random>

int main() {
    std::cout << "+--------------------------------------------------+\n";
    std::cout << "|         GRAPH DEMONSTRATION (10 VERTICES)        |\n";
    std::cout << "+--------------------------------------------------+\n";

    // Create a graph with string vertices
    Graph<std::string, double> graph;

    std::cout << "\n1. ADDING 10 VERTICES:\n";
    std::vector<std::string> cities = {
        "Moscow", "SaintP", "Kazan", "Novosibirsk", "Ekaterinburg",
        "Nizhny", "Samara", "Omsk", "Chelyabinsk", "Rostov"
    };
    
    for (const auto& city : cities) {
        graph.add_vertex(city);
        std::cout << "   Added: " << city << "\n";
    }
    std::cout << "   Graph order (number of vertices): " << graph.order() << "\n";

    std::cout << "\n2. CHECKING VERTICES:\n";
    std::cout << "   Does Moscow exist? " << (graph.has_vertex("Moscow") ? "yes" : "no") << "\n";
    std::cout << "   Does Sochi exist? " << (graph.has_vertex("Sochi") ? "yes" : "no") << "\n";

    std::cout << "\n3. ADDING EDGES (randomized distances):\n";
    std::cout << "   Adding edges between cities with random distances (100-5000 km):\n";
    
    // Добавляем рёбра для создания связного графа
    // Основные связи (чтобы граф был связным)
    graph.add_edge("Moscow", "SaintP", 712.0);
    graph.add_edge("Moscow", "Kazan", 815.0);
    graph.add_edge("Moscow", "Novosibirsk", 3357.0);
    graph.add_edge("Moscow", "Ekaterinburg", 1777.0);
    graph.add_edge("Moscow", "Nizhny", 411.0);
    graph.add_edge("SaintP", "Moscow", 712.0);
    graph.add_edge("SaintP", "Kazan", 1532.0);
    graph.add_edge("Kazan", "Moscow", 815.0);
    graph.add_edge("Kazan", "Samara", 360.0);
    graph.add_edge("Kazan", "Ekaterinburg", 962.0);
    graph.add_edge("Novosibirsk", "Omsk", 669.0);
    graph.add_edge("Novosibirsk", "Moscow", 3357.0);
    graph.add_edge("Ekaterinburg", "Chelyabinsk", 218.0);
    graph.add_edge("Ekaterinburg", "Moscow", 1777.0);
    graph.add_edge("Nizhny", "Moscow", 411.0);
    graph.add_edge("Samara", "Kazan", 360.0);
    graph.add_edge("Omsk", "Novosibirsk", 669.0);
    graph.add_edge("Chelyabinsk", "Ekaterinburg", 218.0);
    graph.add_edge("Rostov", "Moscow", 1086.0);
    graph.add_edge("Rostov", "Samara", 1050.0);
    
    // Дополнительные связи для большей связности
    graph.add_edge("Moscow", "Rostov", 1086.0);
    graph.add_edge("SaintP", "Nizhny", 796.0);
    graph.add_edge("Kazan", "Nizhny", 392.0);
    graph.add_edge("Samara", "Rostov", 1050.0);
    graph.add_edge("Ekaterinburg", "Novosibirsk", 1565.0);
    graph.add_edge("Chelyabinsk", "Omsk", 805.0);
    graph.add_edge("Omsk", "Novosibirsk", 669.0);
    graph.add_edge("Nizhny", "Kazan", 392.0);
    
    std::cout << "   All edges added!\n";
    size_t total_edges = 0;
    for (const auto& v : cities) {
        total_edges += graph.degree(v);
    }
    std::cout << "   Total edges: " << total_edges << "\n";

    std::cout << "\n4. CHECKING EDGES:\n";
    std::cout << "   Does edge Moscow -> SaintP exist? " 
              << (graph.has_edge("Moscow", "SaintP") ? "yes" : "no") << "\n";
    std::cout << "   Does edge SaintP -> Novosibirsk exist? " 
              << (graph.has_edge("SaintP", "Novosibirsk") ? "yes" : "no") << "\n";
    std::cout << "   Does edge Rostov -> Samara exist? " 
              << (graph.has_edge("Rostov", "Samara") ? "yes" : "no") << "\n";

    std::cout << "\n5. VERTEX DEGREE (outgoing):\n";
    for (const auto& v : cities) {
        std::cout << "   Degree of " << v << ": " << graph.degree(v) << "\n";
    }

    std::cout << "\n6. GETTING ALL EDGES FROM MOSCOW:\n";
    auto moscow_edges = graph.edges("Moscow");
    std::cout << "   Edges from Moscow:\n";
    for (const auto& e : moscow_edges) {
        std::cout << "      " << e.from << " -> " << e.to 
                  << " (" << e.distance.value() << " km)\n";
    }

    std::cout << "\n7. GRAPH TRAVERSAL (BFS from Moscow):\n";
    std::cout << "   Traversal order: ";
    auto walk_order = graph.walk("Moscow", [](const std::string& v) {
        std::cout << v << " ";
    });
    std::cout << "\n   Number of vertices visited: " << walk_order.size() << "\n";

    std::cout << "\n8. CONNECTIVITY CHECK:\n";
    std::cout << "   Is the graph strongly connected? " 
              << (graph.is_connected() ? "yes" : "no") << "\n";

    std::cout << "\n9. SHORTEST PATH:\n";
    std::cout << "   Shortest path from Moscow to Novosibirsk:\n";
    auto path = graph.shortest_path("Moscow", "Novosibirsk");
    if (path.empty()) {
        std::cout << "      Path not found!\n";
    } else {
        double total = 0;
        for (size_t i = 0; i < path.size(); ++i) {
            const auto& e = path[i];
            std::cout << "      " << e.from << " -> " << e.to 
                      << " (" << e.distance.value() << " km)";
            total += e.distance.value();
            if (i != path.size() - 1) std::cout << " ->";
            std::cout << "\n";
        }
        std::cout << "      Total distance: " << total << " km\n";
    }

    std::cout << "\n10. SHORTEST PATH FROM ROSTOV TO EKATERINBURG:\n";
    auto path2 = graph.shortest_path("Rostov", "Ekaterinburg");
    if (path2.empty()) {
        std::cout << "      Path not found!\n";
    } else {
        double total = 0;
        for (size_t i = 0; i < path2.size(); ++i) {
            const auto& e = path2[i];
            std::cout << "      " << e.from << " -> " << e.to 
                      << " (" << e.distance.value() << " km)";
            total += e.distance.value();
            if (i != path2.size() - 1) std::cout << " ->";
            std::cout << "\n";
        }
        std::cout << "      Total distance: " << total << " km\n";
    }

    std::cout << "\n11. GETTING ALL VERTICES:\n";
    auto vertices = graph.vertices();
    std::cout << "   List of all vertices (" << vertices.size() << "):\n   ";
    for (const auto& v : vertices) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n12. REMOVING AN EDGE:\n";
    std::cout << "   Removing edge Moscow -> Kazan\n";
    if (graph.remove_edge("Moscow", "Kazan")) {
        std::cout << "   Edge successfully removed!\n";
    }
    std::cout << "   Check: does edge Moscow -> Kazan exist? " 
              << (graph.has_edge("Moscow", "Kazan") ? "yes" : "no") << "\n";

    std::cout << "\n13. REMOVING A VERTEX:\n";
    std::cout << "   Removing vertex Kazan\n";
    if (graph.remove_vertex("Kazan")) {
        std::cout << "   Vertex successfully removed!\n";
    }
    std::cout << "   Graph order after removal: " << graph.order() << "\n";
    std::cout << "   List of vertices after removal: ";
    for (const auto& v : graph.vertices()) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n14. FINDING BEST WAREHOUSE LOCATION:\n";
    std::cout << "   (Finding optimal warehouse location - graph center)\n";
    try {
        auto best = store(graph);
        std::cout << "   Best warehouse location: " << best << "\n";
        
        // Дополнительная информация о расстояниях от склада
        std::cout << "\n   Distances from " << best << ":\n";
        auto vertices_list = graph.vertices();
        for (const auto& v : vertices_list) {
            if (v == best) continue;
            auto dist_path = graph.shortest_path(best, v);
            if (!dist_path.empty()) {
                double total = 0;
                for (const auto& e : dist_path) {
                    total += e.distance.value();
                }
                std::cout << "      to " << v << ": " << total << " km\n";
            } else {
                std::cout << "      to " << v << ": no path\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "   Error: " << e.what() << "\n";
    }

    std::cout << "\n15. GRAPH VISUALIZATION:\n";
    graph.print();

    std::cout << "\n+--------------------------------------------------+\n";
    std::cout << "|              DEMONSTRATION COMPLETE              |\n";
    std::cout << "+--------------------------------------------------+\n";

    return 0;
}