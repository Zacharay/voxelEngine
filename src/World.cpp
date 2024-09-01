
#include "World.hpp"


World::World() {

    int radius = 16;
    int counter = 0;



    for(int i=0;i<radius;i++)
        for(int j=0;j<radius;j++)
            {
                int a =(i+j)%3 +1 ;
                BlockType block = BlockType(a);
                chunks.emplace_back(i,-1,j,block);
                chunks[counter++].generateMesh();
            }

    chunks.emplace_back(2,0,2,BlockType::Air);
    chunks[counter++].generateMesh();
}


World::~World() {

}
const std::vector<Chunk>&World::getChunks()const {
    return chunks;
}

