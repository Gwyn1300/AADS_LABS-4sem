#include "Graph.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "+--------------------------------------------------+\n";
    std::cout << "|         GRAPH DEMONSTRATION                      |\n";
    std::cout << "+--------------------------------------------------+\n";

    // Create a graph with string vertices (cities)
    Graph<std::string, double> graph;

    std::cout << "\n1. ADDING VERTICES:\n";
    std::cout << "   Adding Moscow, Saint Petersburg, Kazan, Novosibirsk\n";
    graph.add_vertex("Moscow");
    graph.add_vertex("SaintP");
    graph.add_vertex("Kazan");
    graph.add_vertex("Novosibirsk");
    std::cout << "   Graph order (number of vertices): " << graph.order() << "\n";

    std::cout << "\n2. CHECKING VERTICES:\n";
    std::cout << "   Does Moscow exist? " << (graph.has_vertex("Moscow") ? "yes" : "no") << "\n";
    std::cout << "   Does Sochi exist? " << (graph.has_vertex("Sochi") ? "yes" : "no") << "\n";

    std::cout << "\n3. ADDING EDGES:\n";
    std::cout << "   Moscow -> SaintP: 712 km\n";
    std::cout << "   Moscow -> Kazan: 815 km\n";
    std::cout << "   Moscow -> Novosibirsk: 3357 km\n";
    std::cout << "   SaintP -> Moscow: 712 km\n";
    std::cout << "   SaintP -> Kazan: 1532 km\n";
    std::cout << "   Kazan -> Moscow: 815 km\n";
    std::cout << "   Kazan -> Novosibirsk: 2917 km\n";
    std::cout << "   Novosibirsk -> Moscow: 3357 km\n";
    
    graph.add_edge("Moscow", "SaintP", 712.0);
    graph.add_edge("Moscow", "Kazan", 815.0);
    graph.add_edge("Moscow", "Novosibirsk", 3357.0);
    graph.add_edge("SaintP", "Moscow", 712.0);
    graph.add_edge("SaintP", "Kazan", 1532.0);
    graph.add_edge("Kazan", "Moscow", 815.0);
    graph.add_edge("Kazan", "Novosibirsk", 2917.0);
    graph.add_edge("Novosibirsk", "Moscow", 3357.0);
    std::cout << "   All edges added!\n";

    std::cout << "\n4. CHECKING EDGES:\n";
    std::cout << "   Does edge Moscow -> SaintP exist? " 
              << (graph.has_edge("Moscow", "SaintP") ? "yes" : "no") << "\n";
    std::cout << "   Does edge SaintP -> Novosibirsk exist? " 
              << (graph.has_edge("SaintP", "Novosibirsk") ? "yes" : "no") << "\n";

    std::cout << "\n5. VERTEX DEGREE (outgoing):\n";
    std::cout << "   Degree of Moscow: " << graph.degree("Moscow") << "\n";
    std::cout << "   Degree of SaintP: " << graph.degree("SaintP") << "\n";
    std::cout << "   Degree of Kazan: " << graph.degree("Kazan") << "\n";
    std::cout << "   Degree of Novosibirsk: " << graph.degree("Novosibirsk") << "\n";

    std::cout << "\n6. GETTING ALL EDGES FROM A VERTEX:\n";
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
    std::cout << "\n   Vector of vertices: ";
    for (const auto& v : walk_order) {
        std::cout << v << " ";
    }
    std::cout << "\n";

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

    std::cout << "\n10. GETTING ALL VERTICES:\n";
    auto vertices = graph.vertices();
    std::cout << "   List of all vertices: ";
    for (const auto& v : vertices) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n11. REMOVING AN EDGE:\n";
    std::cout << "   Removing edge Moscow -> Kazan\n";
    if (graph.remove_edge("Moscow", "Kazan")) {
        std::cout << "   Edge successfully removed!\n";
    }
    std::cout << "   Check: does edge Moscow -> Kazan exist? " 
              << (graph.has_edge("Moscow", "Kazan") ? "yes" : "no") << "\n";

    std::cout << "\n12. REMOVING A VERTEX:\n";
    std::cout << "   Removing vertex Kazan\n";
    if (graph.remove_vertex("Kazan")) {
        std::cout << "   Vertex successfully removed!\n";
    }
    std::cout << "   Graph order after removal: " << graph.order() << "\n";
    std::cout << "   List of vertices: ";
    for (const auto& v : graph.vertices()) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n13. TESTING remove_edge(const Edge&):\n";
    auto edges_from_moscow = graph.edges("Moscow");
    if (!edges_from_moscow.empty()) {
        std::cout << "   Removing first edge from Moscow via Edge version\n";
        graph.remove_edge(edges_from_moscow[0]);
        std::cout << "   Remaining edges from Moscow: ";
        for (const auto& e : graph.edges("Moscow")) {
            std::cout << e.to << "(" << e.distance.value() << ") ";
        }
        std::cout << "\n";
    }

    std::cout << "\n14. TESTING has_edge WITH NON-EXISTENT VERTEX:\n";
    std::cout << "   has_edge(Sochi, Moscow): " 
              << (graph.has_edge("Sochi", "Moscow") ? "true" : "false") << "\n";

    std::cout << "\n15. GRAPH VISUALIZATION:\n";
    graph.print();

    std::cout << "\n+--------------------------------------------------+\n";
    std::cout << "|              DEMONSTRATION COMPLETE              |\n";
    std::cout << "+--------------------------------------------------+\n";

    return 0;
}