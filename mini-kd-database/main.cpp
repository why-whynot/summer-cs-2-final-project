#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "src/KDTree.h"
#include "src/Point.h"
#include "src/Database.h"

using namespace std;

void printMenu() {
    cout << "\n=== KDTree Interactive Menu ===" << endl;
    cout << "1. Insert a point" << endl;
    cout << "2. Search for a point" << endl;
    cout << "3. Remove a point" << endl;
    cout << "4. Range query" << endl;
    cout << "5. Nearest neighbor" << endl;
    cout << "6. k-nearest neighbors" << endl;
    cout << "7. Update a point" << endl;
    cout << "8. Display all points" << endl;
    cout << "9. Clear tree" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

vector<double> readCoordinates(int dimensions) {
    vector<double> coords(dimensions);
    cout << "Enter " << dimensions << " coordinates: ";
    for (int i = 0; i < dimensions; ++i) {
        while (!(cin >> coords[i])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }
    }
    return coords;
}

Point readPoint(int dimensions) {
    vector<double> coords = readCoordinates(dimensions);
    string value;
    cout << "Enter value (optional): ";
    cin.ignore();
    getline(cin, value);
    return Point(coords, value);
}

int main() {
    int dimensions;
    cout << "Enter number of dimensions for KDTree: ";
    while (!(cin >> dimensions) || dimensions <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a positive integer for dimensions: ";
    }
    
    Database db(dimensions);
    
    int choice;
    do {
        printMenu();
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
            continue;
        }
        
        try {
            switch (choice) {
                case 1: {
                    cout << "\n--- Insert Point ---" << endl;
                    Point p = readPoint(dimensions);
                    db.insert(p.getCoordinates(), p.getValue());
                    cout << "Point inserted successfully!" << endl;
                    break;
                }
                
                case 2: {
                    cout << "\n--- Search Point ---" << endl;
                    vector<double> coords = readCoordinates(dimensions);
                    string value = db.getPointValue(coords);
                    if (!value.empty()) {
                        cout << "Point found in tree! Value: '" << value << "'" << endl;
                    } else {
                        cout << "Point not found in tree." << endl;
                    }
                    break;
                }
                
                case 3: {
                    cout << "\n--- Remove Point ---" << endl;
                    vector<double> coords = readCoordinates(dimensions);
                    if (db.remove(coords)) {
                        cout << "Point removed successfully!" << endl;
                    } else {
                        cout << "Point not found for removal." << endl;
                    }
                    break;
                }
                
                case 4: {
                    cout << "\n--- Range Query ---" << endl;
                    cout << "Enter minimum bounds: ";
                    vector<double> min = readCoordinates(dimensions);
                    cout << "Enter maximum bounds: ";
                    vector<double> max = readCoordinates(dimensions);
                    auto results = db.rangeQuery(min, max);
                    cout << "Found " << results.size() << " points in range:" << endl;
                    for (const auto& pr : results) {
                        cout << "  (";
                        for (size_t i = 0; i < pr.first.size(); ++i) {
                            cout << pr.first[i] << (i + 1 < pr.first.size() ? ", " : "");
                        }
                        cout << ") : '" << pr.second << "'" << endl;
                    }
                    break;
                }
                
                case 5: {
                    cout << "\n--- Nearest Neighbor ---" << endl;
                    vector<double> target = readCoordinates(dimensions);
                    try {
                        auto nearest = db.nearestNeighbor(target);
                        cout << "Nearest point: (";
                        for (size_t i = 0; i < nearest.first.size(); ++i) {
                            cout << nearest.first[i] << (i + 1 < nearest.first.size() ? ", " : "");
                        }
                        cout << ") : '" << nearest.second << "'" << endl;
                    } catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                    break;
                }
                
                case 6: {
                    cout << "\n--- k-Nearest Neighbors ---" << endl;
                    vector<double> target = readCoordinates(dimensions);
                    int k;
                    cout << "Enter k (number of neighbors): ";
                    while (!(cin >> k) || k <= 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please enter a positive integer for k: ";
                    }
                    auto results = db.kNearestNeighbors(target, k);
                    cout << "Found " << results.size() << " nearest neighbors:" << endl;
                    for (const auto& pr : results) {
                        cout << "  (";
                        for (size_t i = 0; i < pr.first.size(); ++i) {
                            cout << pr.first[i] << (i + 1 < pr.first.size() ? ", " : "");
                        }
                        cout << ") : '" << pr.second << "'" << endl;
                    }
                    break;
                }
                
                 case 7: {
                    cout << "\n--- Update Point ---" << endl;
                    cout << "Enter coordinates of point to update: ";
                    vector<double> oldCoords = readCoordinates(dimensions);
                    
                    // Check if point exists and get its current value
                    string oldValue = db.getPointValue(oldCoords);
                    if (oldValue.empty()) {
                        cout << "No point found at coordinates (";
                        for (size_t i = 0; i < oldCoords.size(); ++i) {
                            cout << oldCoords[i] << (i + 1 < oldCoords.size() ? ", " : "");
                        }
                        cout << ")" << endl;
                        break;
                    }

                    cout << "Found point with value: '" << oldValue << "'" << endl;
                    cout << "Enter new coordinates (or same if only updating value): "; // Clarify prompt
                    vector<double> newCoords = readCoordinates(dimensions);
                    
                    string updatedValue; // New variable for the updated value
                    cout << "Enter new value (leave empty to keep old value '" << oldValue << "'): ";
                    cin.ignore(); // Consume the newline character left by previous cin
                    getline(cin, updatedValue);
                    if (updatedValue.empty()) {
                        updatedValue = oldValue; // If user enters nothing, keep the old value
                    }

                    bool updated = db.update(oldCoords, newCoords, updatedValue); // Pass the potentially new value
                    
                    if (updated) {
                        cout << "Point updated successfully! Moved from (";
                        for (size_t i = 0; i < oldCoords.size(); ++i) {
                            cout << oldCoords[i] << (i + 1 < oldCoords.size() ? ", " : "");
                        }
                        cout << ") to (";
                        for (size_t i = 0; i < newCoords.size(); ++i) {
                            cout << newCoords[i] << (i + 1 < newCoords.size() ? ", " : "");
                        }
                        cout << ") with value '" << updatedValue << "'" << endl;
                    } else {
                        cout << "Failed to update point." << endl;
                    }
                    break;
                }

                
                case 8: {
                    cout << "\n--- All Points in Tree ---" << endl;
                    cout << "Tree size: " << db.getSize() << endl;
                    if (db.isEmpty()) {
                        cout << "Tree is empty." << endl;
                    } else {
                        cout << "Points in tree:" << endl;
                        db.printAll();
                    }
                    break;
                }
                
                case 9: {
                    cout << "\n--- Clear Tree ---" << endl;
                    db.clear();
                    cout << "Tree cleared successfully!" << endl;
                    break;
                }
                
                case 0: {
                    cout << "Exiting... Thank you!" << endl;
                    break;
                }
                
                default: {
                    cout << "Invalid choice. Please try again." << endl;
                    break;
                }
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
        
    } while (choice != 0);
    
    return 0;
}
