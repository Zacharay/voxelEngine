#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>
#include <Config.hpp>
#include <cfloat>


World::World() {
    m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetFrequency(0.004f);
}
void World::loadChunk(int chunkPosX,int chunkPosZ) {

    glm::ivec2 chunkPos = glm::ivec2(chunkPosX,chunkPosZ);
    //chunk already exists
    if(m_chunks.find(glm::ivec2(chunkPosX, chunkPosZ)) != m_chunks.end()) {
        return;
    }
    ChunkColumn chunk(m_noise,chunkPosX,chunkPosZ);

    ChunkColumn* chunkNx= getChunkColumn(chunkPosX - 1 ,chunkPosZ);
    ChunkColumn* chunkPx= getChunkColumn(chunkPosX + 1 ,chunkPosZ);
    ChunkColumn* chunkNz= getChunkColumn(chunkPosX     ,chunkPosZ - 1);
    ChunkColumn* chunkPz= getChunkColumn(chunkPosX     ,chunkPosZ + 1);

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
ChunkColumn* World::getChunkColumn(int chunkPosX,int chunkPosZ) {
    auto chunkIt = m_chunks.find(glm::ivec2(chunkPosX,chunkPosZ));
    if (chunkIt != m_chunks.end()) {
        return &(chunkIt->second);
    }

    return nullptr;
}

void World::setNeighbours() {
    for(auto& pair : m_chunks) {
        glm::ivec2 pos = pair.first;
        ChunkColumn& chunkColumn = pair.second;



    }
}
