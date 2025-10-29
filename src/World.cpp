#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>
#include <Config.hpp>
#include <cfloat>


World::World() {

    m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetFrequency(0.004f);
    m_noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noise.SetFractalOctaves(4);
    m_noise.SetFractalLacunarity(2.0f);
    m_noise.SetFractalGain(0.5);


    m_treeNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    m_treeNoise.SetFrequency(1000.00f);
    m_treeNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
    m_treeNoise.SetCellularJitter(5.9f);
    m_treeNoise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2);

    m_temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_temperatureNoise.SetFrequency(0.0002f);
    m_temperatureNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_temperatureNoise.SetFractalOctaves(3);
    m_temperatureNoise.SetFractalLacunarity(2.0f);
    m_temperatureNoise.SetFractalGain(0.5f);
    m_temperatureNoise.SetSeed(static_cast<int>(time(0)) + 1);


    m_humidityNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_humidityNoise.SetFrequency(0.0005f);
    m_humidityNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_humidityNoise.SetFractalOctaves(4);
    m_humidityNoise.SetFractalLacunarity(2.0f);
    m_humidityNoise.SetFractalGain(0.5f);
    m_humidityNoise.SetSeed(static_cast<int>(time(0)) + 2);


    m_noise.SetSeed(time(0));
    m_treeNoise.SetSeed(time(0));
    m_chunks.reserve(Config::chunkRadius*Config::chunkRadius);
}

void World::loadChunk(int chunkPosX,int chunkPosZ) {

    glm::ivec2 chunkPos = glm::ivec2(chunkPosX,chunkPosZ);
    //chunk already exists
    if(m_chunks.find(glm::ivec2(chunkPosX, chunkPosZ)) != m_chunks.end()) {
        return;
    }
    ChunkColumn chunk(chunkPosX,chunkPosZ,this);

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
auto divFloor = [](int a, int b) {
    int div = a / b;
    int rem = a % b;
    if (rem < 0) {
        div -= 1;
        rem += b;
    }
    return std::pair<int,int>(div, rem);
};
BlockType World::getBlockAt(glm::ivec3 pos) {
    auto [chunkX, localX] = divFloor(pos.x, Config::chunkSize);
    auto [chunkY, localY] = divFloor(pos.y, Config::chunkSize);
    auto [chunkZ, localZ] = divFloor(pos.z, Config::chunkSize);

    ChunkColumn* column = getChunkColumn(chunkX, chunkZ);
    if (!column) return BlockType::Air;

    return column->getBlockAt(chunkY, localX, localY, localZ);

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

    int meshes_per_frame = 32;
    int regenerated_count = 0;

    for(auto& pair : m_chunks) {
        if (regenerated_count >= meshes_per_frame) {
            break;
        }

        ChunkColumn& chunkColumn = pair.second;
        if(chunkColumn.isMeshDirty) {
            chunkColumn.generateMesh();
            chunkColumn.uploadToGpu();
            regenerated_count++;
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
float World::getHeightNoiseVal(int x, int y) const {
    return m_noise.GetNoise(static_cast<float>(x), static_cast<float>(y)) + 1.0f;
}
float World::getTreeNoiseVal(int x,int y)const {
    return m_treeNoise.GetNoise(static_cast<float>(x), static_cast<float>(y)) ;
}
float World::getHumidityNoiseVal(int x,int y) const {
    return m_humidityNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
}
float World::getTemperatureNoiseVal(int x,int y) const {
    return m_temperatureNoise.GetNoise(static_cast<float>(x), static_cast<float>(y)) ;
}

