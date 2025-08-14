#ifndef DATABASE_H
#define DATABASE_H

#include "KDTree.h"
#include <string>
#include <vector>

class Database {
private:
    KDTree tree;
    int dimensions;

public:
    Database(int dims);
    
    // CRUD Operations
    void insert(const std::vector<double>& coordinates, const std::string& value);
    bool remove(const std::vector<double>& coordinates);
    std::string search(const std::vector<double>& coordinates) const;
    bool update(const std::vector<double>& oldCoords, const std::string& newValue);
    bool update(const std::vector<double>& oldCoords, const std::vector<double>& newCoords, const std::string& newValue);
    
    // Query Operations
    std::vector<std::pair<std::vector<double>, std::string>> rangeQuery(
        const std::vector<double>& min, const std::vector<double>& max) const;
    std::pair<std::vector<double>, std::string> nearestNeighbor(
        const std::vector<double>& target) const;
    std::vector<std::pair<std::vector<double>, std::string>> kNearestNeighbors(
        const std::vector<double>& target, int k) const;
    
    // Utility
    bool isEmpty() const;
    int getSize() const;
    int getDimensions() const;
    void clear();
    void printAll() const;
    
    // Enhanced update with old value tracking
    std::pair<std::vector<double>, std::string> updateAndGetOld(const std::vector<double>& oldCoords, 
                                                               const std::vector<double>& newCoords, 
                                                               const std::string& newValue);
    
    // Get point value by coordinates
    std::string getPointValue(const std::vector<double>& coordinates) const;
};

#endif // DATABASE_H
