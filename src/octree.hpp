#pragma once
#include <iostream> 
#include <vector>
#include <array>
#include <thread>
#include <cmath>
#include <algorithm>
#include <mutex>
using namespace std;

// Data Structure for Octree and Point

class Point {
public:
    float x, y, z;

    Point() : x(0), y(0), z(0) {}
    Point(float x, float y, float z) : x(x), y(y), z(z) {}
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    float dot(const Point& other)  { return x*other.x + y*other.y + z*other.z; }
    Point sub(const Point& other) { return {x-other.x, y-other.y, z-other.z}; }
    Point cross(const Point& other) { return {y*other.z-z*other.y, z*other.x-x*other.z, x*other.y-y*other.x}; }
};

class Face{
public:
    Point v1, v2, v3;

    Face() : v1(Point()), v2(Point()), v3(Point()) {}
    Face(Point v1, Point v2, Point v3) : v1(v1), v2(v2), v3(v3) {}
};

class Voxel{
public:
    Point center;
    float size;
    bool fill;

    Voxel() : center(Point()), size(0), fill(false) {}
    Voxel(Point center, float size, bool fill) : center(center), size(size), fill(fill) {}

    // Function
    bool isInside(Point p);
};

class OctreeNode{
public:
    // Data 
    Voxel info;
    array<OctreeNode*, 8> children;

    // CTOR and DTOR
    OctreeNode() : info(){children.fill(nullptr);}
    OctreeNode(Voxel v) : info(v){children.fill(nullptr);}
    ~OctreeNode() = default;

    // Functions
    bool isOneElmt(){
        for(int i = 0; i < 8; i++){
            if(children[i] != nullptr){
                return false;
            }
        }
        return true;
    }

    void divide();
};