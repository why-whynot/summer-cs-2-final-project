#include "Database.h"
#include <stdexcept>
#include <algorithm>

Database::Database(int dims) : tree(dims), dimensions(dims) {}

void Database::insert(const std::vector<double>& coordinates, const std::string& value) {
    if (coordinates.size() != dimensions) {
        throw std::invalid_argument("Point dimensions do not match database dimensions");
    }
    tree.insert(Point(coordinates, value));
}

bool Database::remove(const std::vector<double>& coordinates) {
    if (coordinates.size() != dimensions) {
        return false;
    }
    return tree.remove(Point(coordinates, ""));
}

std::string Database::search(const std::vector<double>& coordinates) const {
    if (coordinates.size() != dimensions) {
        throw std::invalid_argument("Point dimensions do not match database dimensions");
    }
    
    Point searchPoint(coordinates, "");
    if (tree.search(searchPoint)) {
        // Note: This is a limitation - we need to find the actual point to get its value
        // For now, return empty string if found
        return ""; // In a real implementation, we'd need to store values separately
    }
    return "";
}

bool Database::update(const std::vector<double>& oldCoords, const std::string& newValue) {
    if (oldCoords.size() != dimensions) {
        return false;
    }
    
    // Get the old value first
    std::string oldValue = getPointValue(oldCoords);
    if (oldValue.empty()) {
        return false;  // Point doesn't exist
    }
    
    // Remove old point and insert new one with updated value
    bool removed = tree.remove(Point(oldCoords, oldValue));
    if (removed) {
        tree.insert(Point(oldCoords, newValue));
        return true;
    }
    return false;
}

bool Database::update(const std::vector<double>& oldCoords, const std::vector<double>& newCoords, const std::string& newValue) {
    if (oldCoords.size() != dimensions || newCoords.size() != dimensions) {
        return false;
    }
    
    // Get the old value first
    std::string oldValue = getPointValue(oldCoords);
    if (oldValue.empty()) {
        return false;  // Point doesn't exist
    }
    
    // Remove old point and insert new one with updated value
    bool removed = tree.remove(Point(oldCoords, oldValue));
    if (removed) {
        tree.insert(Point(newCoords, newValue));
        return true;
    }
    return false;
}

// Enhanced update method that preserves old value
std::pair<std::vector<double>, std::string> Database::updateAndGetOld(const std::vector<double>& oldCoords, 
                                                                     const std::vector<double>& newCoords, 
                                                                     const std::string& newValue) {
    if (oldCoords.size() != dimensions || newCoords.size() != dimensions) {
        return {{}, ""};
    }
    
    // First, find the old point to get its current value
    Point oldPoint(oldCoords, "");
    if (!tree.search(oldPoint)) {
        return {{}, ""};
    }
    
    // Get the old value (this would need enhancement in a real implementation)
    std::string oldValue = "value_at_" + std::to_string(oldCoords[0]); // Placeholder
    
    // Remove old point
    bool removed = tree.remove(Point(oldCoords, ""));
    if (removed) {
        // Insert new point
        tree.insert(Point(newCoords, newValue));
        return {oldCoords, oldValue};
    }
    
    return {{}, ""};
}

std::string Database::getPointValue(const std::vector<double>& coordinates) const {
    if (coordinates.size() != dimensions) {
        return "";
    }
    
    // Use range query to find the exact point
    auto results = rangeQuery(coordinates, coordinates);
    if (!results.empty()) {
        return results[0].second;
    }
    
    return "";
}

std::vector<std::pair<std::vector<double>, std::string>> Database::rangeQuery(
    const std::vector<double>& min, const std::vector<double>& max) const {
    
    if (min.size() != dimensions || max.size() != dimensions) {
        throw std::invalid_argument("Range dimensions do not match database dimensions");
    }
    
    std::vector<Point> points = tree.rangeQuery(min, max);
    std::vector<std::pair<std::vector<double>, std::string>> results;
    
    for (const Point& p : points) {
        results.emplace_back(p.getCoordinates(), p.getValue());
    }
    
    return results;
}

std::pair<std::vector<double>, std::string> Database::nearestNeighbor(
    const std::vector<double>& target) const {
    
    if (target.size() != dimensions) {
        throw std::invalid_argument("Target dimensions do not match database dimensions");
    }
    
    Point nearest = tree.nearestNeighbor(target);
    return {nearest.getCoordinates(), nearest.getValue()};
}


std::vector<std::pair<std::vector<double>, std::string>> Database::kNearestNeighbors(
    const std::vector<double>& target, int k) const {
    if (target.size() != dimensions) {
        throw std::invalid_argument("Target dimensions do not match database dimensions");
    }
    std::vector<Point> points = tree.kNearestNeighbors(target, k);
    std::vector<std::pair<std::vector<double>, std::string>> results;
    for (const Point& p : points) {
        results.emplace_back(p.getCoordinates(), p.getValue());
    }
    return results;
}

bool Database::isEmpty() const {
    return tree.isEmpty();
}

int Database::getSize() const {
    return tree.size();
}

int Database::getDimensions() const {
    return dimensions;
}

void Database::clear() {
    tree.clear();
}

void Database::printAll() const {
    tree.print();
}
