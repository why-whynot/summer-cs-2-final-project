#ifndef KDTREE_H
#define KDTREE_H

#include "Point.h"
#include <vector>
#include <memory>
#include <algorithm>

class KDNode {
public:
    Point point;
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;
    
    KDNode(const Point& p);
};

class KDTree {
private:
    std::unique_ptr<KDNode> root;
    int dimensions;
    
    // Helper methods
    std::unique_ptr<KDNode> buildTree(std::vector<Point>& points, int depth, int left, int right);
    int partition(std::vector<Point>& points, int left, int right, int pivot, int dimension);
    double findMedian(std::vector<Point>& points, int left, int right, int dimension);
    
    // Search helpers
    void rangeSearch(const KDNode* node, const std::vector<double>& min, 
                    const std::vector<double>& max, int depth, std::vector<Point>& results) const;
    void nearestNeighbor(const KDNode* node, const std::vector<double>& target, 
                        int depth, Point& best, double& bestDist) const;
    
    // Insert helpers
    std::unique_ptr<KDNode> insertNode(std::unique_ptr<KDNode> node, const Point& point, int depth);
    
    // Delete helpers
    std::unique_ptr<KDNode> deleteNode(std::unique_ptr<KDNode> node, const Point& point, int depth);
    std::unique_ptr<KDNode> findMin(std::unique_ptr<KDNode> node, int dimension, int currentDim);
    
    // Utility
    double distance(const Point& p1, const Point& p2) const;
    double distance(const Point& p, const std::vector<double>& coords) const;

public:
    KDTree(int dims);
    
    // Core operations
    void insert(const Point& point);
    bool remove(const Point& point);
    bool search(const Point& point) const;
    void update(const Point& oldPoint, const Point& newPoint);
    
    // Query operations
    std::vector<Point> rangeQuery(const std::vector<double>& min, const std::vector<double>& max) const;
    Point nearestNeighbor(const std::vector<double>& target) const;
    std::vector<Point> kNearestNeighbors(const std::vector<double>& target, int k) const;
    
    // Utility
    bool isEmpty() const;
    void clear();
    void print() const;
    int size() const;
    
    // Getters
    int getDimensions() const;
    
private:
    int countNodes(const KDNode* node) const;
    void printInOrder(const KDNode* node) const;
};

#endif // KDTREE_H
