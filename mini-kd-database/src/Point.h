#ifndef POINT_H
#define POINT_H

#include <vector>
#include <string>
#include <iostream>

class Point {
private:
    std::vector<double> coordinates;
    std::string value;

public:
    // Constructor
    Point(const std::vector<double>& coords, const std::string& val = "");
    
    // Getters
    const std::vector<double>& getCoordinates() const;
    double getCoordinate(int dimension) const;
    const std::string& getValue() const;
    
    // Setters
    void setValue(const std::string& val);
    void setCoordinate(int dimension, double value);
    
    // Utility functions
    double distanceTo(const Point& other) const;
    double distanceTo(const std::vector<double>& coords) const;
    bool equals(const Point& other) const;
    
    // Dimension
    int getDimensions() const;
    
    // Print
    void print() const;
};

#endif // POINT_H
