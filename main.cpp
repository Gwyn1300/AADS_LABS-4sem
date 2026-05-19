#include "Graph.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║               ДЕМОНСТРАЦИЯ РАБОТЫ ГРАФА                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    // Создаём граф с вершинами-строками (городами)
    Graph<std::string, double> graph;

    std::cout << "\n1. ДОБАВЛЕНИЕ ВЕРШИН:\n";
    std::cout << "   Добавляем Москву, Питер, Казань, Новосибирск\n";
    graph.add_vertex("Москва");
    graph.add_vertex("Питер");
    graph.add_vertex("Казань");
    graph.add_vertex("Новосибирск");
    std::cout << "   Порядок графа (количество вершин): " << graph.order() << "\n";

    std::cout << "\n2. ПРОВЕРКА НАЛИЧИЯ ВЕРШИН:\n";
    std::cout << "   Есть ли Москва? " << (graph.has_vertex("Москва") ? "да" : "нет") << "\n";
    std::cout << "   Есть ли Сочи? " << (graph.has_vertex("Сочи") ? "да" : "нет") << "\n";

    std::cout << "\n3. ДОБАВЛЕНИЕ РЁБЕР:\n";
    std::cout << "   Москва -> Питер: 712 км\n";
    std::cout << "   Москва -> Казань: 815 км\n";
    std::cout << "   Москва -> Новосибирск: 3357 км\n";
    std::cout << "   Питер -> Москва: 712 км\n";
    std::cout << "   Питер -> Казань: 1532 км\n";
    std::cout << "   Казань -> Москва: 815 км\n";
    std::cout << "   Казань -> Новосибирск: 2917 км\n";
    std::cout << "   Новосибирск -> Москва: 3357 км\n";
    
    graph.add_edge("Москва", "Питер", 712.0);
    graph.add_edge("Москва", "Казань", 815.0);
    graph.add_edge("Москва", "Новосибирск", 3357.0);
    graph.add_edge("Питер", "Москва", 712.0);
    graph.add_edge("Питер", "Казань", 1532.0);
    graph.add_edge("Казань", "Москва", 815.0);
    graph.add_edge("Казань", "Новосибирск", 2917.0);
    graph.add_edge("Новосибирск", "Москва", 3357.0);
    std::cout << "   Все рёбра добавлены!\n";

    std::cout << "\n4. ПРОВЕРКА НАЛИЧИЯ РЁБЕР:\n";
    std::cout << "   Есть ли ребро Москва -> Питер? " 
              << (graph.has_edge("Москва", "Питер") ? "да" : "нет") << "\n";
    std::cout << "   Есть ли ребро Питер -> Новосибирск? " 
              << (graph.has_edge("Питер", "Новосибирск") ? "да" : "нет") << "\n";

    std::cout << "\n5. СТЕПЕНЬ ВЕРШИНЫ (исходящая):\n";
    std::cout << "   Степень Москвы: " << graph.degree("Москва") << "\n";
    std::cout << "   Степень Питера: " << graph.degree("Питер") << "\n";
    std::cout << "   Степень Казани: " << graph.degree("Казань") << "\n";
    std::cout << "   Степень Новосибирска: " << graph.degree("Новосибирск") << "\n";

    std::cout << "\n6. ПОЛУЧЕНИЕ ВСЕХ РЁБЕР ИЗ ВЕРШИНЫ:\n";
    auto moscow_edges = graph.edges("Москва");
    std::cout << "   Рёбра из Москвы:\n";
    for (const auto& e : moscow_edges) {
        std::cout << "      " << e.from << " -> " << e.to 
                  << " (" << e.distance.value() << " км)\n";
    }

    std::cout << "\n7. ОБХОД ГРАФА (BFS от Москвы):\n";
    std::cout << "   Порядок обхода: ";
    auto walk_order = graph.walk("Москва", [](const std::string& v) {
        std::cout << v << " ";
    });
    std::cout << "\n   Вектор вершин: ";
    for (const auto& v : walk_order) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n8. ПРОВЕРКА СВЯЗНОСТИ:\n";
    std::cout << "   Граф сильно связный? " 
              << (graph.is_connected() ? "да" : "нет") << "\n";

    std::cout << "\n9. ПОИСК КРАТЧАЙШЕГО ПУТИ:\n";
    std::cout << "   Кратчайший путь из Москвы в Новосибирск:\n";
    auto path = graph.shortest_path("Москва", "Новосибирск");
    if (path.empty()) {
        std::cout << "      Путь не найден!\n";
    } else {
        double total = 0;
        for (size_t i = 0; i < path.size(); ++i) {
            const auto& e = path[i];
            std::cout << "      " << e.from << " -> " << e.to 
                      << " (" << e.distance.value() << " км)";
            total += e.distance.value();
            if (i != path.size() - 1) std::cout << " ->";
            std::cout << "\n";
        }
        std::cout << "      Общее расстояние: " << total << " км\n";
    }

    std::cout << "\n10. ПОЛУЧЕНИЕ ВСЕХ ВЕРШИН:\n";
    auto vertices = graph.vertices();
    std::cout << "   Список всех вершин: ";
    for (const auto& v : vertices) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n11. УДАЛЕНИЕ РЕБРА:\n";
    std::cout << "   Удаляем ребро Москва -> Казань\n";
    if (graph.remove_edge("Москва", "Казань")) {
        std::cout << "   Ребро успешно удалено!\n";
    }
    std::cout << "   Проверка: есть ли ребро Москва -> Казань? " 
              << (graph.has_edge("Москва", "Казань") ? "да" : "нет") << "\n";

    std::cout << "\n12. УДАЛЕНИЕ ВЕРШИНЫ:\n";
    std::cout << "   Удаляем вершину Казань\n";
    if (graph.remove_vertex("Казань")) {
        std::cout << "   Вершина успешно удалена!\n";
    }
    std::cout << "   Порядок графа после удаления: " << graph.order() << "\n";
    std::cout << "   Список вершин: ";
    for (const auto& v : graph.vertices()) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "\n13. ПРОВЕРКА РАБОТЫ remove_edge(const Edge&):\n";
    auto edges_from_moscow = graph.edges("Москва");
    if (!edges_from_moscow.empty()) {
        std::cout << "   Удаляем первое ребро из Москвы через Edge-версию\n";
        graph.remove_edge(edges_from_moscow[0]);
        std::cout << "   Проверка: ";
        for (const auto& e : graph.edges("Москва")) {
            std::cout << e.to << "(" << e.distance.value() << ") ";
        }
        std::cout << "\n";
    }

    std::cout << "\n14. ПОПЫТКА ДОБАВИТЬ НЕСУЩЕСТВУЮЩУЮ ВЕРШИНУ В has_edge:\n";
    std::cout << "   has_edge(Сочи, Москва): " 
              << (graph.has_edge("Сочи", "Москва") ? "true" : "false") << "\n";

    std::cout << "\n15. ВИЗУАЛИЗАЦИЯ ГРАФА:\n";
    graph.print();

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    return 0;
}