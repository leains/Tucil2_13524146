#include "octree.hpp"
#include "dnc.hpp"
#include "viewer/viewer.hpp"
#include "io.hpp"

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
    cout << "PILIH OPSI : \n1. PROSES FILE\n2. VIEW FILE\nMasukkan opsi: ";
    cin >> opsi;
    // Input File
    while (opsi != "1" && opsi != "2"){
        cout << "\nOpsi invalid!!!. Masukkan opsi: ";
        cin >> opsi;
    }
    if (opsi == "1"){
        // Proses File
        cout << "Masukkan nama file: ";
        string filename;
        cin >> filename;
        Root root;
        root = prosesFile(filename);
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

        int depth;
        cout << "Masukkan Depth: ";
        cin >> depth;
        Root::NODE_UNUSED.assign(depth,0);
        Root::NODE_USED.assign(depth,0);
        root.algorithm(&octree,0,depth);
        
        Result result = root.konversiVoxel();
        string fend;
        cout << "Masukkan nama file akhir: ";
        cin >> fend;
        tulisFile(fend,result);
        cout << "File berhasil diproses dan disimpan sebagai " << fend << ".obj\n";
        cout << "---INFOMRASI TAMBAHAN---\n" << "Banyaknya Voxel Yang terbentuk: " << Root::VOXEL_COUNT 
        << "\nBanyaknya Vertex: " << Root::VERTICES_COUNT << "\nBanyaknya Faces: " << Root::FACES_COUNT;
        
        cout << "\nBanyaknya Node Octree yang terbentuk\n";
        for (int i=1;i<=depth;i++){cout << i << ": " << Root::NODE_UNUSED[i]+Root::NODE_USED[i] << "\n";}
        cout << "Banyaknya Node Octree yang tidak ditelusuri\n";
        for (int i=1;i<=depth;i++){cout << i << ": " << Root::NODE_UNUSED[i] << "\n";}

    }else if (opsi == "2"){
        // View File
        cout << "Masukkan nama file: ";
        string filename;
        cin >> filename;
        viewer(filename);
    }
}