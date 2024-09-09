
#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>


World::World() {

    constexpr int radius = 1;

    int height = 1;

    for(int x = -radius; x <= radius; x++) {
        for(int z = -radius; z <= radius; z++) {
                m_chunks.insert({glm::ivec2(x,z),ChunkColumn(height,x,z)});
                height += 1;
        }
    }


   // setNeighbours();
/*
    for(auto &pair:m_chunks) {
        Chunk &chunk = pair.second;
        chunk.generateMesh();
    }
*/


}


World::~World() {

}
/*
void World::setNeighbours() {
    for(auto& pair : m_chunks) {
        glm::ivec3 pos = pair.first;
        Chunk& chunk = pair.second;


        Chunk* chunkNx = nullptr;
        Chunk* chunkPx = nullptr;
        Chunk* chunkNy = nullptr;
        Chunk* chunkPy = nullptr;
        Chunk* chunkNz = nullptr;
        Chunk* chunkPz = nullptr;


        auto itNx = m_chunks.find(glm::ivec3(pos.x - 1, pos.y, pos.z));
        if (itNx != m_chunks.end()) {
            chunkNx = &(itNx->second);
        }


        auto itPx = m_chunks.find(glm::ivec3(pos.x + 1, pos.y, pos.z));
        if (itPx != m_chunks.end()) {
            chunkPx = &(itPx->second);
        }


        auto itNy = m_chunks.find(glm::ivec3(pos.x, pos.y - 1, pos.z));
        if (itNy != m_chunks.end()) {
            chunkNy = &(itNy->second);
        }


        auto itPy = m_chunks.find(glm::ivec3(pos.x, pos.y + 1, pos.z));
        if (itPy != m_chunks.end()) {
            chunkPy = &(itPy->second);
        }

        auto itNz = m_chunks.find(glm::ivec3(pos.x, pos.y, pos.z - 1));
        if (itNz != m_chunks.end()) {
            chunkNz = &(itNz->second);

        }


        auto itPz = m_chunks.find(glm::ivec3(pos.x, pos.y, pos.z + 1));
        if (itPz != m_chunks.end()) {
            chunkPz = &(itPz->second);
        }

        chunk.setNeighboursChunks(chunkNx, chunkPx, chunkNy, chunkPy, chunkNz, chunkPz);
    }
}
*/
const ChunkMap &World::getChunks()const {
    return m_chunks;
}

