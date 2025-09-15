#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>
#include <Config.hpp>
#include <cfloat>


World::World() {


    m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetFrequency(0.0015f);


    // for(int chunkPosX = -Config::chunkRadius; chunkPosX < Config::chunkRadius; chunkPosX++) {
    //     for(int chunkPosZ = -Config::chunkRadius; chunkPosZ < Config::chunkRadius; chunkPosZ++) {
    //
    //     }
    // }
    //
    //
    // setNeighbours();
    //
    // for(auto &pair:m_chunks) {
    //     ChunkColumn &chunk = pair.second;
    //     chunk.generateMesh();
    // }



}


World::~World() {

}

void World::setNeighbours() {
    for(auto& pair : m_chunks) {
        glm::ivec2 pos = pair.first;
        ChunkColumn& chunkColumn = pair.second;



    }
}
void World::loadChunk(int chunkPosX,int chunkPosZ) {

    glm::ivec2 chunkPos = glm::ivec2(chunkPosX,chunkPosZ);
    //chunk already exists
    if(m_chunks.find(glm::ivec2(chunkPosX, chunkPosZ)) != m_chunks.end()) {
        return;
    }
    ChunkColumn chunk(m_noise,chunkPosX,chunkPosZ);

    ChunkColumn* chunkNx= nullptr;
    ChunkColumn* chunkPx= nullptr;
    ChunkColumn* chunkNz= nullptr;
    ChunkColumn* chunkPz= nullptr;



    auto itNx = m_chunks.find(glm::ivec2(chunkPosX - 1,  chunkPosZ));
    if (itNx != m_chunks.end()) {
        chunkNx = &(itNx->second);
    }


    auto itPx = m_chunks.find(glm::ivec2(chunkPosX + 1,  chunkPosZ));
    if (itPx != m_chunks.end()) {
        chunkPx = &(itPx->second);
    }



    auto itNz = m_chunks.find(glm::ivec2(chunkPosX,  chunkPosZ - 1));
    if (itNz != m_chunks.end()) {
        chunkNz = &(itNz->second);

    }


    auto itPz = m_chunks.find(glm::ivec2(chunkPosX,chunkPosZ + 1));
    if (itPz != m_chunks.end()) {
        chunkPz = &(itPz->second);
    }

    auto [it, inserted] = m_chunks.emplace(chunkPos, std::move(chunk));
    ChunkColumn* chunkPtr = &it->second;

    chunkPtr->setNeighbouringChunks(chunkNx, chunkPx, chunkNz, chunkPz);



    if(chunkNx) {
        chunkNx->setNeighbourPx(chunkPtr);
        chunkNx->isMeshDirty = true;
    }
    if(chunkPx) {
        chunkPx->setNeighbourNx(chunkPtr);
        chunkPx->isMeshDirty = true;
    }
    if(chunkNz) {
        chunkNz->setNeighbourPz(chunkPtr);
        chunkNz->isMeshDirty = true;
    }
    if(chunkPz) {
        chunkPz->setNeighbourNz(chunkPtr);
        chunkPz->isMeshDirty = true;
    }


}
void World::unloadFarChunks(int playerChunkX,int playerChunkZ) {
    const int unloadRadius = static_cast<int>(Config::chunkRadius) + 1;
    std::vector<glm::ivec2> toRemove;
    toRemove.reserve(32);

    for (auto &p : m_chunks) {
        const glm::ivec2 pos = p.first;
        int dx = pos.x - playerChunkX;
        int dz = pos.y - playerChunkZ;
        if (std::abs(dx) > unloadRadius || std::abs(dz) > unloadRadius) {
            toRemove.push_back(pos);
        }
    }

    for (auto &pos : toRemove) {
        auto it = m_chunks.find(pos);
        if (it == m_chunks.end()) continue;

        it->second.destroyGL();
        it->second.disconnectNeighbours();
        m_chunks.erase(it);
    }
}
void World::regenerateMeshes() {

    for(auto& pair : m_chunks) {
        ChunkColumn& chunkColumn = pair.second;
        if(chunkColumn.isMeshDirty) {
            chunkColumn.generateMesh();
        }
    }
}
const ChunkMap &World::getChunks()const {
    return m_chunks;
}

