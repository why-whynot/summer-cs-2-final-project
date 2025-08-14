# Mini Database with K-D Tree

## Overview
This project implements a mini database using K-D tree (k-dimensional tree) for efficient storage and retrieval of multi-dimensional data. The implementation supports CRUD operations (Create, Read, Update, Delete) and provides efficient range queries and nearest neighbor searches.

## Features
- **Multi-dimensional data storage**: Supports any number of dimensions
- **Balanced K-D tree**: Maintains balance during insertions and deletions
- **CRUD operations**: Complete Create, Read, Update, Delete functionality
- **Range queries**: Efficient searching within multi-dimensional ranges
- **Nearest neighbor search**: Find closest points in k-dimensional space
- **k-nearest neighbors**: Find k closest points to a target
- **Memory management**: Proper cleanup and memory leak prevention
- **Interactive CLI**: Command-line interface for testing and usage

## Project Structure
```
mini-kd-database/
├── main.cpp              # Interactive CLI program
├── test_kdtree.cpp       # Comprehensive test suite
├── src/
│   ├── Database.h        # Database interface
│   ├── Database.cpp      # Database implementation
│   ├── KDTree.h          # K-D tree class declaration
│   ├── KDTree.cpp        # K-D tree implementation
│   ├── Point.h           # Point structure for multi-dimensional data
│   └── Point.cpp         # Point implementation
├── kdtree_app            # Compiled executable (interactive CLI)
├── test_kdtree           # Compiled test suite
└── README.md             # This file
```

## Building and Running

### Prerequisites
- C++14 or later compiler (C++11 insufficient for std::make_unique)
- Standard C++ library with std::make_unique support
- CMake (optional, but recommended for build management)

### Build Commands with CMake
```bash
cd mini-kd-database

make && ./kdtree_app  # Compile the interactive CLI
```
### Build Commands without CMake
```bash
cd mini-kd-database

g++ main.cpp src/Database.cpp src/KDTree.cpp src/Point.cpp -o kdtree_app -Isrc -std=c++14
g++ test_kdtree.cpp src/Database.cpp src/KDTree.cpp src/Point.cpp -o test_kdtree -Isrc -std=c++14

./kdtree_app
```



### Run Tests
```bash
./test_kdtree
./test_update  # Test the updated functionality
```

## Usage Examples

### Interactive CLI
When you run `./kdtree_app`, you'll get an interactive menu:
```
=== KDTree Interactive Menu ===
1. Insert a point
2. Search for a point
3. Remove a point
4. Range query
5. Nearest neighbor
6. k-nearest neighbors
7. Update a point
8. Display all points
9. Clear tree
0. Exit
```
### Testing
## Testing insertion
enter 3
enter 1
In "Enter 3 coordinates" enter: 1 2 3
In "Enter value" enter: point 1
enter 1
In "Enter 3 coordinates" enter: 4 5 6
In "Enter value" enter: point 2
enter 1
In "Enter 3 coordinates" enter: 7 8 9
In "Enter value" enter: point 3
## results 
enter 8
the following should be displayed
Tree size: 3
Points in tree:
Point(1, 2, 3) = point 1
Point(4, 5, 6) = point 2
Point(7, 8, 9) = point 3

## Testing search
enter 2
In "Enter 3 coordinates" enter: 1 2 3
## results
Point found in tree!

## Testing range query
enter 4
In "Enter minimum bound: Enter 3 coordinates" enter: 0 0 0
In "Enter maximum bound: Enter 3 coordinates" enter: 10 10 10
## results
Found 3 points in range:
  Point(1, 2, 3) = point 1
  Point(4, 5, 6) = point 2
  Point(7, 8, 9) = point 3

## Testing nearest neighbor
enter 5
In "Enter 3 coordinates" enter: 1 1 1
## results
Nearest neighbor: Point(1, 2, 3) = point 1

## Testing k nearest neighbors
enter 6
In "Enter 3 coordinates" enter: 1 1 1
In "Enter k" enter: 2
## results
Found 2 nearest neighbors:
  Point(1, 2, 3) = point 1
  Point(4, 5, 6) = point 2

## Update a point
enter 7
In "Enter old point coordinates: Enter 3 coordinates" enter: 1 2 3
In "Enter new point coordinates: Enter 3 coordinates" enter: 10 11 12
## results
Point moved successfully from (1, 2, 3) to (10, 11, 12)

## Display all points
enter 8
## results
--- All Points in Tree ---
Tree size: 3
Points in tree:
Point(4, 5, 6) = point 2
Point(7, 8, 9) = point 3
Point(10, 11, 12) = point 1

## Remove point
enter 3
enter 4 5 6
## results
--- Remove Point ---
Enter 3 coordinates: 4 5 6
Point removed successfully!

## Clear tree
enter 9
## results
--- Clear Tree ---
Tree cleared successfully!

## Technical Details

### K-D Tree Properties
- **Space partitioning**: Recursively splits k-dimensional space
- **Median splitting**: Uses median values for balanced trees
- **Dimension cycling**: Alternates dimensions at each level
- **O(log n) operations**: Average case for balanced trees

### Supported Data Types
- Coordinates: double precision floating point
- Values: string or double (templated support)

## Performance Characteristics
- **Insertion**: O(log n) average, O(n) worst case
- **Search**: O(log n) average, O(n) worst case
- **Range query**: O(√n + k) where k is number of results
- **Memory**: O(n) space complexity

## Testing
The project includes comprehensive tests covering:
- Basic CRUD operations
- Range queries
- Nearest neighbor searches
- k-nearest neighbors
- Edge cases and error handling
- Memory management

Run the test suite with:
```bash
./test_kdtree
```

## Future Enhancements
- Support for bulk operations
- Persistence to disk with file I/O
- Concurrent access support with threading
- Additional query types (spherical queries, etc.)
- Performance benchmarking suite
- Template-based generic value types
- Distance metric customization (Manhattan, cosine, etc.)
- Visualization tools for tree structure