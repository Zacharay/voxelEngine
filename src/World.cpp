
#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>


World::World() {

    constexpr int radius = 8;

    int height = 1;

    for(int x = -radius; x <= radius; x++) {
        for(int z = -radius; z <= radius; z++) {
                m_chunks.insert({glm::ivec2(x,z),ChunkColumn(height,x,z)});
                height += 1;
        }
    }


    setNeighbours();

    for(auto &pair:m_chunks) {
        ChunkColumn &chunk = pair.second;
        chunk.generateMesh();
    }



}


World::~World() {

}

void World::setNeighbours() {
    for(auto& pair : m_chunks) {
        glm::ivec2 pos = pair.first;
        ChunkColumn& chunkColumn = pair.second;


        ChunkColumn* chunkNx = nullptr;
        ChunkColumn* chunkPx = nullptr;
        ChunkColumn* chunkNz = nullptr;
        ChunkColumn* chunkPz = nullptr;


        auto itNx = m_chunks.find(glm::ivec2(pos.x - 1,  pos.y));
        if (itNx != m_chunks.end()) {
            chunkNx = &(itNx->second);
        }


        auto itPx = m_chunks.find(glm::ivec2(pos.x + 1,  pos.y));
        if (itPx != m_chunks.end()) {
            chunkPx = &(itPx->second);
        }



        auto itNz = m_chunks.find(glm::ivec2(pos.x,  pos.y - 1));
        if (itNz != m_chunks.end()) {
            chunkNz = &(itNz->second);

        }


        auto itPz = m_chunks.find(glm::ivec2(pos.x,pos.y + 1));
        if (itPz != m_chunks.end()) {
            chunkPz = &(itPz->second);
        }

        chunkColumn.setNeighbouringChunks(chunkNx, chunkPx, chunkNz, chunkPz);
    }
}

const ChunkMap &World::getChunks()const {
    return m_chunks;
}

