#include "octree.hpp"
#include "dnc.hpp"
#include "io.hpp"
#include <chrono>

int main(){
    float xmax = -INFINITY, xmin = INFINITY, ymax = -INFINITY, ymin = INFINITY, zmax = -INFINITY, zmin = INFINITY;
    // 2 OPSI : Proses File atau View File
    string opsi;
    cout <<R"(
                 __      _____________.____   _________  ________      _____  ___________._._._. 
                /  \    /  \_   _____/|    |  \_   ___ \ \_____  \    /     \ \_   _____/| | | | 
                \   \/\/   /|    __)_ |    |  /    \  \/  /   |   \  /  \ /  \ |    __)_ | | | | 
                \        / |        \|    |__\     \____/    |    \/    Y    \|        \ \|\|\| 
                \__/\  / /_______  /|_______ \______  /\_______  /\____|__  /_______  / ______ 
                    \/          \/         \/      \/         \/         \/        \/  \/\/\/
    )" << "\n";
    cout << "===PROGRAM VOXELIZATION===\n[Ketentuan]\n1. File .obj hanya berisi vertex dan face dengan format standar (v x y z untuk vertex dan f v1 v2 v3 untuk face)\n2. Jika ada format yang dilanggar, program akan menghiraukannya\n3. File .obj tidak boleh berisi string selain faces dan vertices: Hal ini akan mengakibatkan error di program ini. (Termasuk tidak boleh ada kata pengantar di awal)\n" 
    << "\n===[Default: File .obj disimpan pada folder test/]===\nPilihan Input Nama File:\n1. File name only (tanpa .obj)\n2. Path (Jika ingin memasukkan path lengkap berikut dengan .obj)\nMasukkan opsi: ";
    cin >> opsi;
    // Input File
    while (opsi != "1" && opsi != "2"){
        cout << "\nOpsi invalid!!!. Masukkan opsi: ";
        cin >> opsi;
    }
    cout << "Masukkan nama file: ";
    string filename;
    cin >> filename;
    Root root;
    // Proses File
    try{
        if (opsi == "1"){root=prosesFile("../test/" + filename + ".obj");}
        else if (opsi == "2"){root = prosesFile(filename);}
    }catch (exception e) {cout<<"[!] File Corrupt : Format tidak sesuai"; return 0;}
    auto start = chrono::steady_clock::now(); // Start Timer        

    // Cube Awal
    Result result;
    int depth;
    try{
        for (Point p : root.vertices){
            xmax = max(xmax, p.x);
            xmin = min(xmin, p.x);
            ymax = max(ymax, p.y);
            ymin = min(ymin, p.y);
            zmax = max(zmax, p.z);
            zmin = min(zmin, p.z);
        }
        Point center = Point((xmax+xmin)/2, (ymax+ymin)/2, (zmax+zmin)/2);
        float size = max({xmax-xmin, ymax-ymin, zmax-zmin}) * 1.1f; // Tambahkan sedikit margin
        // Algoritma Divide And Conquer
        OctreeNode octree;
        Voxel v = Voxel(center, size, true);
        octree = OctreeNode(v);

        cout << "Masukkan Depth: ";
        cin >> depth;
        Root::NODE_UNUSED.assign(depth,0);
        Root::NODE_USED.assign(depth,0);
        root.algorithm(&octree,0,depth);
        
        result = root.konversiVoxel();
    }catch(exception e){cout << "[!] Error saat proses algoritma: " << e.what() << "\nPastikan depth yang dimasukkan tidak terlalu besar untuk objek yang diproses\n"; return 0;}
    auto end = chrono::steady_clock::now(); // End Timer
    chrono::duration<double> durations = end-start;

    string fend;
    if (opsi == "1"){
        cout << "Masukkan nama file akhir [Default: File name only]: ";
        cin >> fend;
        tulisFile("../test/" + fend + ".obj",result);}
    else if (opsi == "2"){
        cout << "Masukkan path file akhir [<path>/<filename>.obj]: ";
        cin >> fend;
        tulisFile(fend,result);}
    cout << "\n---INFORMASI OUTPUT---\n" << "Banyaknya Voxel Yang terbentuk: " << Root::VOXEL_COUNT 
    << "\nBanyaknya Vertex yang terbentuk: " << Root::VERTICES_COUNT << "\nBanyaknya Faces yang terbentuk: " << Root::FACES_COUNT << "\nKedalaman Octree yang digunakan: " << depth;
    
    cout << "\nBanyaknya Node Octree yang terbentuk: \n";
    for (int i=0;i<depth;i++){cout << i+1 << ": " << Root::NODE_UNUSED[i]+Root::NODE_USED[i] << "\n";}
    cout << "Banyaknya Node Octree yang tidak ditelusuri: \n";
    for (int i=0;i<depth;i++){cout << i+1 << ": " << Root::NODE_UNUSED[i] << "\n";}

    cout << "Waktu Komputasi Algoritma: " << durations.count() << "s\n\n";
    cout << "[File berhasil diproses dan disimpan sebagai " << fend << ".obj di dalam folder test/]\n";
    
}