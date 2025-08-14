#include "KDTree.h"
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <stdexcept>

KDNode::KDNode(const Point& p) : point(p), left(nullptr), right(nullptr) {}

KDTree::KDTree(int dims) : dimensions(dims) {
    if (dims <= 0) {
        throw std::invalid_argument("Dimensions must be positive");
    }
}

void KDTree::insert(const Point& point) {
    if (point.getDimensions() != dimensions) {
        throw std::invalid_argument("Point dimensions do not match tree dimensions");
    }
    root = insertNode(std::move(root), point, 0);
}

std::unique_ptr<KDNode> KDTree::insertNode(std::unique_ptr<KDNode> node, 
                                           const Point& point, int depth) {
    if (!node) {
        return std::make_unique<KDNode>(point);
    }
    
    int currentDim = depth % dimensions;
    
    if (point.getCoordinate(currentDim) < node->point.getCoordinate(currentDim)) {
        node->left = insertNode(std::move(node->left), point, depth + 1);
    } else {
        node->right = insertNode(std::move(node->right), point, depth + 1);
    }
    
    return std::move(node);
}

bool KDTree::remove(const Point& point) {
    if (point.getDimensions() != dimensions) {
        return false;
    }
    
    int initialSize = size();
    root = deleteNode(std::move(root), point, 0);
    return size() < initialSize;
}

std::unique_ptr<KDNode> KDTree::deleteNode(std::unique_ptr<KDNode> node, 
                                           const Point& point, int depth) {
    if (!node) {
        return nullptr;
    }
    
    int currentDim = depth % dimensions;
    
    if (point.equals(node->point)) {
        if (!node->left && !node->right) {
            return nullptr;
        }
        
        if (!node->left) {
            return std::move(node->right);
        }
        
        if (!node->right) {
            return std::move(node->left);
        }
        
        // Find minimum in right subtree
        auto minNode = findMin(std::move(node->right), currentDim, (depth + 1) % dimensions);
        node->point = minNode->point;
        node->right = deleteNode(std::move(node->right), minNode->point, depth + 1);
        
    } else if (point.getCoordinate(currentDim) < node->point.getCoordinate(currentDim)) {
        node->left = deleteNode(std::move(node->left), point, depth + 1);
    } else {
        node->right = deleteNode(std::move(node->right), point, depth + 1);
    }
    
    return std::move(node);
}

std::unique_ptr<KDNode> KDTree::findMin(std::unique_ptr<KDNode> node, 
                                        int dimension, int currentDim) {
    if (!node) {
        return nullptr;
    }
    
    if (dimension == currentDim) {
        if (!node->left) {
            return std::move(node);
        }
        return findMin(std::move(node->left), dimension, (currentDim + 1) % dimensions);
    }
    
    auto leftMin = findMin(std::move(node->left), dimension, (currentDim + 1) % dimensions);
    auto rightMin = findMin(std::move(node->right), dimension, (currentDim + 1) % dimensions);
    
    auto minNode = std::move(node);
    
    if (leftMin && leftMin->point.getCoordinate(dimension) < minNode->point.getCoordinate(dimension)) {
        minNode = std::move(leftMin);
    }
    
    if (rightMin && rightMin->point.getCoordinate(dimension) < minNode->point.getCoordinate(dimension)) {
        minNode = std::move(rightMin);
    }
    
    return minNode;
}

bool KDTree::search(const Point& point) const {
    const KDNode* current = root.get();
    int depth = 0;
    
    while (current) {
        if (point.equals(current->point)) {
            return true;
        }
        
        int currentDim = depth % dimensions;
        if (point.getCoordinate(currentDim) < current->point.getCoordinate(currentDim)) {
            current = current->left.get();
        } else {
            current = current->right.get();
        }
        depth++;
    }
    
    return false;
}

std::vector<Point> KDTree::rangeQuery(const std::vector<double>& min, 
                                     const std::vector<double>& max) const {
    if (min.size() != dimensions || max.size() != dimensions) {
        throw std::invalid_argument("Range dimensions do not match tree dimensions");
    }
    
    std::vector<Point> results;
    rangeSearch(root.get(), min, max, 0, results);
    return results;
}

void KDTree::rangeSearch(const KDNode* node, const std::vector<double>& min, 
                        const std::vector<double>& max, int depth, 
                        std::vector<Point>& results) const {
    if (!node) return;
    
    const Point& point = node->point;
    bool inRange = true;
    
    for (int i = 0; i < dimensions; ++i) {
        if (point.getCoordinate(i) < min[i] || point.getCoordinate(i) > max[i]) {
            inRange = false;
            break;
        }
    }
    
    if (inRange) {
        results.push_back(point);
    }
    
    int currentDim = depth % dimensions;
    
    if (min[currentDim] <= point.getCoordinate(currentDim)) {
        rangeSearch(node->left.get(), min, max, depth + 1, results);
    }
    
    if (max[currentDim] >= point.getCoordinate(currentDim)) {
        rangeSearch(node->right.get(), min, max, depth + 1, results);
    }
}

Point KDTree::nearestNeighbor(const std::vector<double>& target) const {
    if (target.size() != dimensions) {
        throw std::invalid_argument("Target dimensions do not match tree dimensions");
    }
    
    if (!root) {
        throw std::runtime_error("Tree is empty");
    }
    
    Point best = root->point;
    double bestDist = distance(root->point, target);
    
    nearestNeighbor(root.get(), target, 0, best, bestDist);
    return best;
}

void KDTree::nearestNeighbor(const KDNode* node, const std::vector<double>& target, 
                            int depth, Point& best, double& bestDist) const {
    if (!node) return;
    
    double dist = distance(node->point, target);
    if (dist < bestDist) {
        bestDist = dist;
        best = node->point;
    }
    
    int currentDim = depth % dimensions;
    double diff = target[currentDim] - node->point.getCoordinate(currentDim);
    
    const KDNode* near = diff < 0 ? node->left.get() : node->right.get();
    const KDNode* far = diff < 0 ? node->right.get() : node->left.get();
    
    nearestNeighbor(near, target, depth + 1, best, bestDist);
    
    if (std::abs(diff) < bestDist) {
        nearestNeighbor(far, target, depth + 1, best, bestDist);
    }
}

double KDTree::distance(const Point& p1, const Point& p2) const {
    return p1.distanceTo(p2);
}

double KDTree::distance(const Point& p, const std::vector<double>& coords) const {
    return p.distanceTo(coords);
}

bool KDTree::isEmpty() const {
    return !root;
}

void KDTree::clear() {
    root.reset();
}

int KDTree::size() const {
    return countNodes(root.get());
}

int KDTree::countNodes(const KDNode* node) const {
    if (!node) return 0;
    return 1 + countNodes(node->left.get()) + countNodes(node->right.get());
}

int KDTree::getDimensions() const {
    return dimensions;
}

void KDTree::print() const {
    printInOrder(root.get());
}

void KDTree::printInOrder(const KDNode* node) const {
    if (!node) return;
    
    printInOrder(node->left.get());
    node->point.print();
    printInOrder(node->right.get());
}

void KDTree::update(const Point& oldPoint, const Point& newPoint) {
    if (oldPoint.getDimensions() != dimensions || newPoint.getDimensions() != dimensions) {
        throw std::invalid_argument("Point dimensions do not match tree dimensions");
    }
    
    // If the points are the same, just update the value
    if (oldPoint.equals(newPoint)) {
        // Find the existing point and update its value
        KDNode* current = root.get();
        int depth = 0;
        
        while (current) {
            if (oldPoint.equals(current->point)) {
                current->point.setValue(newPoint.getValue());
                return;
            }
            
            int currentDim = depth % dimensions;
            if (oldPoint.getCoordinate(currentDim) < current->point.getCoordinate(currentDim)) {
                current = current->left.get();
            } else {
                current = current->right.get();
            }
            depth++;
        }
        return;
    }
    
    // If coordinates changed, remove old and insert new
    if (remove(oldPoint)) {
        insert(newPoint);
    }
}

std::vector<Point> KDTree::kNearestNeighbors(const std::vector<double>& target, int k) const {
    if (target.size() != dimensions) {
        throw std::invalid_argument("Target dimensions do not match tree dimensions");
    }
    
    if (k <= 0) {
        return {};
    }
    
    if (!root) {
        return {};
    }
    
    // Use a max-heap to keep track of k nearest neighbors
    auto cmp = [](const std::pair<Point, double>& a, const std::pair<Point, double>& b) {
        return a.second < b.second;
    };
    std::priority_queue<std::pair<Point, double>, std::vector<std::pair<Point, double>>, decltype(cmp)> maxHeap(cmp);
    
    // Helper lambda for k-nearest neighbor search
    std::function<void(const KDNode*, int)> search = [&](const KDNode* node, int depth) {
        if (!node) return;
        
        double dist = distance(node->point, target);
        
        if (maxHeap.size() < static_cast<size_t>(k)) {
            maxHeap.emplace(node->point, dist);
        } else if (dist < maxHeap.top().second) {
            maxHeap.pop();
            maxHeap.emplace(node->point, dist);
        }
        
        int currentDim = depth % dimensions;
        double diff = target[currentDim] - node->point.getCoordinate(currentDim);
        
        const KDNode* near = diff < 0 ? node->left.get() : node->right.get();
        const KDNode* far = diff < 0 ? node->right.get() : node->left.get();
        
        search(near, depth + 1);
        
        if (maxHeap.size() < static_cast<size_t>(k) || std::abs(diff) < maxHeap.top().second) {
            search(far, depth + 1);
        }
    };
    
    search(root.get(), 0);
    
    std::vector<Point> result;
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top().first);
        maxHeap.pop();
    }
    
    // Reverse to get ascending order by distance
    std::reverse(result.begin(), result.end());
    return result;
}

std::unique_ptr<KDNode> KDTree::buildTree(std::vector<Point>& points, int depth, int left, int right) {
    if (left >= right) {
        return nullptr;
    }
    
    int currentDim = depth % dimensions;
    
    // Find median using nth_element
    int mid = left + (right - left) / 2;
    std::nth_element(points.begin() + left, points.begin() + mid, points.begin() + right,
        [currentDim](const Point& a, const Point& b) {
            return a.getCoordinate(currentDim) < b.getCoordinate(currentDim);
        });
    
    auto node = std::make_unique<KDNode>(points[mid]);
    node->left = buildTree(points, depth + 1, left, mid);
    node->right = buildTree(points, depth + 1, mid + 1, right);
    
    return node;
}

double KDTree::findMedian(std::vector<Point>& points, int left, int right, int dimension) {
    if (left >= right) {
        return 0.0;
    }
    
    int mid = left + (right - left) / 2;
    std::nth_element(points.begin() + left, points.begin() + mid, points.begin() + right,
        [dimension](const Point& a, const Point& b) {
            return a.getCoordinate(dimension) < b.getCoordinate(dimension);
        });
    
    return points[mid].getCoordinate(dimension);
}

int KDTree::partition(std::vector<Point>& points, int left, int right, int pivot, int dimension) {
    std::swap(points[pivot], points[right - 1]);
    int store = left;
    
    for (int i = left; i < right - 1; ++i) {
        if (points[i].getCoordinate(dimension) < points[right - 1].getCoordinate(dimension)) {
            std::swap(points[i], points[store]);
            ++store;
        }
    }
    
    std::swap(points[store], points[right - 1]);
    return store;
}
