#include "octree.hpp"

bool Voxel::isInside(Point P){
    return (P.x >= center.x - size/2 && P.x <= center.x + size/2) && 
    (P.y >= center.y - size/2 && P.y <= center.y + size/2) && 
    (P.z >= center.z - size/2 && P.z <= center.z + size/2);
}

void OctreeNode::divide(){
    float newSize = info.size/2.0f;
    float offset = info.size/4.0f;

    for(int i = 0; i < 8; i++){
        float ox = (i & 1) ? offset : -offset;
        float oy = (i & 2) ? offset : -offset;
        float oz = (i & 4) ? offset : -offset;

        Point childP = Point(info.center.x+ox, info.center.y+oy, info.center.z+oz);
        Voxel temp = Voxel(childP,info.size/2.0f, true);

        this->children[i] = new OctreeNode(temp);
    }
}
