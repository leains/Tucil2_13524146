#pragma once
#include "octree.hpp"

extern mutex VoxelMutex; 
const int THREAD_LIMIT = 4; 

class Index{
public:
    int a,b,c;
    
    Index(int a, int b, int c) : a(a),b(b),c(c) {}
};

class Result{
public:
    // Hasil Vertices dan Faces
    vector<Point> vertices;
    vector<Index> faces;
};

class Root{
public:
    // Counter
    static int VOXEL_COUNT, VERTICES_COUNT, FACES_COUNT;
    static vector<int> NODE_USED, NODE_UNUSED; 
    // DATA
    // Vektor berisi vertices
    vector<Point> vertices;
    // Vektor berisi faces
    vector<Face> faces;
    // Vektor berisi voxel
    vector<Voxel> voxels;

    // FUNCTIONS
    // Cek apakah faces ada di dalam voxel?
    bool isContain(Voxel v);

    // Pembagian octree secara rekursif
    void algorithm(OctreeNode* octree, int depth, int target);

    // Pengisian Voxel
    void fillVoxel(OctreeNode* octree, int depth, int target);

    // Konversi Voxel ke Faces dan Vertices
    Result konversiVoxel();
};
