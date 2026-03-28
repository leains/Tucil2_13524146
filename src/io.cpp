#include "io.hpp"


Root prosesFile(string file){
    Root R;
    int idxVertices = 1, idxFaces = 1;
    ifstream File("../test/" + file + ".obj");
    
    if (!File.is_open()) {
        cerr << "Error: Cannot open file " << file << ".obj" << endl;
        return R;
    }

    string line;
    while (getline(File, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        char c;
        iss >> c;

        if (c == 'v' || c == 'V') {
            float x, y, z;
            if (iss >> x >> y >> z) {
                Point p = Point(x, y, z);
                R.vertices.push_back(p);
                idxVertices++;
            }
        } else if (c == 'f' || c == 'F') {
            int v1, v2, v3;
            if (iss >> v1 >> v2 >> v3) {
                Point f1,f2,f3;
                f1 = Point(R.vertices[v1-1].x, R.vertices[v1-1].y, R.vertices[v1-1].z);
                f2 = Point(R.vertices[v2-1].x, R.vertices[v2-1].y, R.vertices[v2-1].z);
                f3 = Point(R.vertices[v3-1].x, R.vertices[v3-1].y, R.vertices[v3-1].z);
                Face f = Face(f1,f2,f3);
                R.faces.push_back(f);
                idxFaces++;
            }
        }
    }

    File.close();
    return R;
}

void tulisFile(string file, Result result){
    ofstream File("../test/" + file + ".obj");
    
    for (Point v: result.vertices){
        File << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }

    for (Index f: result.faces){
        File << "f " << f.a << " " << f.b << " " << f.c << "\n";
    }
}

