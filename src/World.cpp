
#include "World.hpp"


World::World() {

    int radius = 4;
    int counter = 0;
    for(int i=0;i<radius;i++)
        for(int j=0;j<radius;j++)
            for(int k=0;k<radius;k++) {
                chunks.emplace_back(i,-1,k);
                chunks[counter++].generateMesh();
            }


}


World::~World() {

}
const std::vector<Chunk>&World::getChunks()const {
    return chunks;
}

