#include <iostream>
#include <vector>
#include "src/KDTree.h"
#include "src/Point.h"

int main() {
    std::cout << "=== KDTree Testing ===" << std::endl;
    
    // Test 1: Basic KDTree creation and insertion
    std::cout << "\nTest 1: Basic KDTree creation and insertion" << std::endl;
    KDTree tree(2);
    
    // Insert some points
    tree.insert(Point({2.0, 3.0}, "A"));
    tree.insert(Point({5.0, 4.0}, "B"));
    tree.insert(Point({9.0, 6.0}, "C"));
    tree.insert(Point({4.0, 7.0}, "D"));
    tree.insert(Point({8.0, 1.0}, "E"));
    tree.insert(Point({7.0, 2.0}, "F"));
    
    std::cout << "Tree size: " << tree.size() << std::endl;
    std::cout << "Tree empty? " << (tree.isEmpty() ? "Yes" : "No") << std::endl;
    
    // Test 2: Search functionality
    std::cout << "\nTest 2: Search functionality" << std::endl;
    Point searchPoint({5.0, 4.0});
    std::cout << "Searching for (5,4): " << (tree.search(searchPoint) ? "Found" : "Not found") << std::endl;
    
    Point notFoundPoint({1.0, 1.0});
    std::cout << "Searching for (1,1): " << (tree.search(notFoundPoint) ? "Found" : "Not found") << std::endl;
    
    // Test 3: Range query
    std::cout << "\nTest 3: Range query" << std::endl;
    std::vector<double> min = {3.0, 2.0};
    std::vector<double> max = {8.0, 6.0};
    auto rangeResults = tree.rangeQuery(min, max);
    std::cout << "Points in range [3,2] to [8,6]: " << rangeResults.size() << std::endl;
    for (const auto& p : rangeResults) {
        std::cout << "  - ";
        p.print();
    }
    
    // Test 4: Nearest neighbor
    std::cout << "\nTest 4: Nearest neighbor" << std::endl;
    std::vector<double> target = {6.0, 5.0};
    try {
        Point nearest = tree.nearestNeighbor(target);
        std::cout << "Nearest to (6,5): ";
        nearest.print();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    // Test 5: k-nearest neighbors
    std::cout << "\nTest 5: k-nearest neighbors" << std::endl;
    auto kNearest = tree.kNearestNeighbors(target, 3);
    std::cout << "3 nearest to (6,5): " << kNearest.size() << std::endl;
    for (const auto& p : kNearest) {
        std::cout << "  - ";
        p.print();
    }
    
    // Test 6: Update functionality
    std::cout << "\nTest 6: Update functionality" << std::endl;
    Point oldPoint({2.0, 3.0});
    Point newPoint({2.5, 3.5});
    tree.update(oldPoint, newPoint);
    std::cout << "Updated (2,3) to (2.5,3.5)" << std::endl;
    std::cout << "Searching for old point (2,3): " << (tree.search(oldPoint) ? "Found" : "Not found") << std::endl;
    std::cout << "Searching for new point (2.5,3.5): " << (tree.search(newPoint) ? "Found" : "Not found") << std::endl;
    
    // Test 7: Remove functionality
    std::cout << "\nTest 7: Remove functionality" << std::endl;
    Point removePoint({9.0, 6.0});
    std::cout << "Before removal - size: " << tree.size() << std::endl;
    bool removed = tree.remove(removePoint);
    std::cout << "Removed (9,6): " << (removed ? "Success" : "Failed") << std::endl;
    std::cout << "After removal - size: " << tree.size() << std::endl;
    
    // Test 8: Clear and empty check
    std::cout << "\nTest 8: Clear and empty check" << std::endl;
    tree.clear();
    std::cout << "After clear - size: " << tree.size() << std::endl;
    std::cout << "Tree empty? " << (tree.isEmpty() ? "Yes" : "No") << std::endl;
    
    std::cout << "\n=== All tests completed successfully! ===" << std::endl;
    
    return 0;
}
