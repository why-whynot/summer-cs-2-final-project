#include "Point.h"
#include <cmath>
#include <stdexcept>

Point::Point(const std::vector<double>& coords, const std::string& val) 
    : coordinates(coords), value(val) {}

const std::vector<double>& Point::getCoordinates() const {
    return coordinates;
}

double Point::getCoordinate(int dimension) const {
    if (dimension < 0 || dimension >= coordinates.size()) {
        throw std::out_of_range("Dimension out of range");
    }
    return coordinates[dimension];
}

const std::string& Point::getValue() const {
    return value;
}

void Point::setValue(const std::string& val) {
    value = val;
}

void Point::setCoordinate(int dimension, double value) {
    if (dimension < 0 || dimension >= coordinates.size()) {
        throw std::out_of_range("Dimension out of range");
    }
    coordinates[dimension] = value;
}

double Point::distanceTo(const Point& other) const {
    return distanceTo(other.coordinates);
}

double Point::distanceTo(const std::vector<double>& coords) const {
    if (coords.size() != coordinates.size()) {
        throw std::invalid_argument("Dimension mismatch");
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < coordinates.size(); ++i) {
        double diff = coordinates[i] - coords[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

bool Point::equals(const Point& other) const {
    if (coordinates.size() != other.coordinates.size()) {
        return false;
    }
    
    for (size_t i = 0; i < coordinates.size(); ++i) {
        if (std::abs(coordinates[i] - other.coordinates[i]) > 1e-10) {
            return false;
        }
    }
    return true;
}

int Point::getDimensions() const {
    return coordinates.size();
}

void Point::print() const {
    std::cout << "Point(";
    for (size_t i = 0; i < coordinates.size(); ++i) {
        std::cout << coordinates[i];
        if (i < coordinates.size() - 1) std::cout << ", ";
    }
    std::cout << ") = " << value << std::endl;
}
