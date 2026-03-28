#include "dnc.hpp"
mutex VoxelMutex;
int Root::VOXEL_COUNT = 0, Root::VERTICES_COUNT = 0, Root::FACES_COUNT = 0;
vector<int> Root::NODE_UNUSED, Root::NODE_USED;

//AABB
bool axisTest (Point Axis,Point P1, Point P2, Point P3, Point half){
    float f1 = Axis.dot(P1);
    float f2 = Axis.dot(P2);
    float f3 = Axis.dot(P3);
    float r = half.x*abs(Axis.x) + half.y*abs(Axis.y) + half.z*abs(Axis.z);
    float mi = min({f1,f2,f3});
    float ma = max({f1,f2,f3});
    return !(mi > r || ma < -r);
}

bool AABB(Face F, Voxel V){
    Point P1 = F.v1.sub(V.center);
    Point P2 = F.v2.sub(V.center);
    Point P3 = F.v3.sub(V.center);
    Point h = Point (V.size/2.0f,  V.size/2.0f, V.size/2.0f);

    // 1
    if (!axisTest({1,0,0}, P1,P2,P3, h)) return false;
    if (!axisTest({0,1,0}, P1,P2,P3, h)) return false;
    if (!axisTest({0,0,1}, P1,P2,P3, h)) return false;

    // 2
    Point e1 = P2.sub(P1), e2 = P3.sub(P2), e3 = P1.sub(P3);
    Point n = e1.cross(e2);
    if ((n.x!=0||n.y!=0||n.z!=0) && !axisTest(n, P1,P2,P3, h)) return false;

    // 3
    Point axes[3] = {{1,0,0},{0,1,0},{0,0,1}};
    for (Point ax : axes){
        Point a1 = e1.cross(ax);
        Point a2 = e2.cross(ax);
        Point a3 = e3.cross(ax);
        if ((a1.x!=0||a1.y!=0||a1.z!=0) && !axisTest(a1, P1,P2,P3, h)) return false;
        if ((a2.x!=0||a2.y!=0||a2.z!=0) && !axisTest(a2, P1,P2,P3, h)) return false;
        if ((a3.x!=0||a3.y!=0||a3.z!=0) && !axisTest(a3, P1,P2,P3, h)) return false;
    }

    return true;
}


// Cek Intersect
bool Root::isContain(Voxel v){
    // Cek apakah ada faces yang masuk ke dalam voxel (Intersect)
    for (Face f : faces){
        if (AABB(f,v)) return true;
    }
    return false;
}

void Root::algorithm(OctreeNode* octree, int depth, int target){
    // Base : Stop Rekursif kalau sudah kosong
    if (depth == target || octree->info.fill == false){
        if (octree->info.fill){
            lock_guard<mutex> lock(VoxelMutex);
            voxels.push_back(octree->info);
            VOXEL_COUNT++;
        }
        return;
    }else{
        // Rekursif : Bagi octree menjadi 8 bagian
        octree->divide();

        // Cek apakah ada faces yang masuk ke dalam voxel
        for (int i = 0; i < 8; i++){
            if (isContain(octree->children[i]->info)){
                octree->children[i]->info.fill = true;
            }else{
                octree->children[i]->info.fill = false;
            }
        }
        cerr << "depth=" << depth << " selesai\n";
        // Concurrency untuk tiap rekursif (Thread hanya sampai 3 sesuai dengan THREAD_LIMIT di dnc.hpp)
        if (depth < THREAD_LIMIT){
            thread threads[8];
            for (int i = 0; i < 8; i++){
                if (octree->children[i]->info.fill){
                    NODE_USED[depth+1]++;
                    threads[i] = thread(&Root::algorithm, this, octree->children[i], depth+1, target);
                }else{
                    NODE_UNUSED[depth+1]++;
                    threads[i] = thread([]{});
                }
            }
            for (int i = 0; i < 8; i++){
                threads[i].join();
            }
        }else{
            for (int i = 0; i < 8; i++){
                if (octree->children[i]->info.fill){
                    NODE_USED[depth+1]++;
                    algorithm(octree->children[i], depth+1, target);
                }else{NODE_UNUSED[depth+1]++;}
            }
        }
    }
}

Result Root::konversiVoxel(){
    Result result;
    int idx = 0;
    for (Voxel v: voxels){
        Point c = v.center;
        float s = v.size/2.0f;
        // 8 sudut (Vertices)
        Point p1 = Point(c.x-s, c.y-s, c.z-s);
        Point p2 = Point(c.x+s, c.y-s, c.z-s);
        Point p3 = Point(c.x+s, c.y+s, c.z-s);
        Point p4 = Point(c.x-s, c.y+s, c.z-s);
        Point p5 = Point(c.x-s, c.y-s, c.z+s);
        Point p6 = Point(c.x+s, c.y-s, c.z+s);
        Point p7 = Point(c.x+s, c.y+s, c.z+s);
        Point p8 = Point(c.x-s, c.y+s, c.z+s);
        result.vertices.push_back(p1);
        result.vertices.push_back(p2);
        result.vertices.push_back(p3);
        result.vertices.push_back(p4);
        result.vertices.push_back(p5);
        result.vertices.push_back(p6);  
        result.vertices.push_back(p7);
        result.vertices.push_back(p8);
        
        // 6 Sisi (12 Faces : Segitiga)
        result.faces.push_back(Index(idx+1,idx+2,idx+3));
        result.faces.push_back(Index(idx+1,idx+3,idx+4));
        result.faces.push_back(Index(idx+5,idx+6,idx+7));
        result.faces.push_back(Index(idx+5,idx+7,idx+8));
        result.faces.push_back(Index(idx+1,idx+2,idx+6));
        result.faces.push_back(Index(idx+1,idx+6,idx+5));
        result.faces.push_back(Index(idx+4,idx+3,idx+7));
        result.faces.push_back(Index(idx+4,idx+7,idx+8));
        result.faces.push_back(Index(idx+1,idx+4,idx+8));
        result.faces.push_back(Index(idx+1,idx+8,idx+5));
        result.faces.push_back(Index(idx+2,idx+3,idx+7));
        result.faces.push_back(Index(idx+2,idx+7,idx+6));

        idx += 8;
        VERTICES_COUNT+=8;
        FACES_COUNT+=12;
    }

    return result;
}
