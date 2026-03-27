#pragma once
#include "octree.hpp"

// Cube akan diperlakukan sebagai octree "dalam kutip", maksudnya bahwa fungsi2 yang akan dijalankan dan aksi2 yang akan dilakukan itu didefinisikan dalam octree.

// 1. Rekursif (Divide n Conquer)
// 2. Tiap rekursif akan mengecek base, which is -> Apakah cuman ada satu atau kosong?
// 3. Jika belum, maka dibagi terus. Jika sudah satu atau kosong, maka akan dimasukkans sebagai octree (INSERT)
// 4. Hasil Akhir :: Octree yang berisi which is titik2 dari voxel yang sudah dibuat (Koordinat)
// 5. Merge semua voxel dengan membaut faces baru (PRNYA!!!) -> Hitung Faces
// 6. Hitung Vertices juga dan semua ketentuan yang ada.
// 7. Jika sudah ada informasi tentang vetices dan faces, maka voxelization sudah berhasil. 

// OUTPUT : Sebuah DATA STRUCTURE yang berisi informasi mengenai objek baru. (Vertices, Faces, dkk)
// Masukkin ke .obj File akan diimplementasikan di file lain : Dari objek ke file
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
