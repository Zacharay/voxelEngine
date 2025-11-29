#include "World.hpp"
#include "FastNoiseLite.h"
#include <iostream>
#include <Config.hpp>
#include <WorldGenerator.hpp>




World::World() {

    m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetFrequency(0.009f);
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


    unsigned int numThreads = std::max(1u, std::thread::hardware_concurrency() - 1);
    m_threadPool = std::make_unique<ThreadPool>(numThreads);

    m_chunks.reserve(Config::chunkRadius*Config::chunkRadius);
}

void World::loadChunk(int chunkPosX,int chunkPosZ) {

    glm::ivec2 chunkPos = glm::ivec2(chunkPosX,chunkPosZ);
    //chunk already exists
    if(m_chunks.find(glm::ivec2(chunkPosX, chunkPosZ)) != m_chunks.end()) {
        return;
    }


    ChunkColumn* chunkNx= getChunkColumn(chunkPosX - 1 ,chunkPosZ);
    ChunkColumn* chunkPx= getChunkColumn(chunkPosX + 1 ,chunkPosZ);
    ChunkColumn* chunkNz= getChunkColumn(chunkPosX     ,chunkPosZ - 1);
    ChunkColumn* chunkPz= getChunkColumn(chunkPosX     ,chunkPosZ + 1);

    auto [it, inserted] = m_chunks.emplace(chunkPos, std::make_unique<ChunkColumn>(chunkPosX,chunkPosZ,*this));
    ChunkColumn* chunkPtr = it->second.get();

    chunkPtr->setNeighbouringChunks(chunkNx, chunkPx, chunkNz, chunkPz);


    if(chunkNx) {
        chunkNx->setNeighbourPx(chunkPtr);
        chunkNx->setMeshDirty(true);
    }
    if(chunkPx) {
        chunkPx->setNeighbourNx(chunkPtr);
        chunkPx->setMeshDirty(true);
    }
    if(chunkNz) {
        chunkNz->setNeighbourPz(chunkPtr);
        chunkNz->setMeshDirty(true);
    }
    if(chunkPz) {
        chunkPz->setNeighbourNz(chunkPtr);
        chunkPz->setMeshDirty(true);
    }


}
BlockType World::getBlockAt(glm::ivec3 pos) {

    const int chunkXIndex = pos.x >> Config::chunkSizeShift;
    const int chunkZIndex = pos.z >> Config::chunkSizeShift;
    const int localX = pos.x & Config::chunkSizeMask;
    const int localZ = pos.z  & Config::chunkSizeMask;

    ChunkColumn* column = getChunkColumn(chunkXIndex, chunkZIndex);
    if (!column) return BlockType::Air;

    return column->getBlockAt(localX, pos.y, localZ);

}
void World::setBlockAt(glm::ivec3 pos,BlockType blockType) {
    const int chunkXIndex = pos.x >> Config::chunkSizeShift;
    const int chunkZIndex = pos.z >> Config::chunkSizeShift;
    const int localX = pos.x & Config::chunkSizeMask;
    const int localZ = pos.z  & Config::chunkSizeMask;

    ChunkColumn* column = getChunkColumn(chunkXIndex, chunkZIndex);
    if (!column) return ;

    //check if rebuild neighbouring chunk if on border of the current chunk
    if(localX == 0) column->getNeighbourNx()->setMeshDirty(true);
    if(localX == Config::chunkSize-1 ) column->getNeighbourPx()->setMeshDirty(true);
    if(localZ == 0) column->getNeighbourNz()->setMeshDirty(true);
    if(localZ == Config::chunkSize-1 ) column->getNeighbourPz()->setMeshDirty(true);

    return column->setBlockAt(localX, pos.y, localZ,blockType);
}


void World::processChunkMeshes() {

    const int MAX_UPLOADS_PER_FRAME =1000;
    int uploadedCount = 0;


    std::lock_guard<std::mutex> lock(m_chunkMapMutex);

    for (auto& pair : m_chunks) {
        if (uploadedCount >= MAX_UPLOADS_PER_FRAME) break;

        std::unique_ptr<ChunkColumn>& column = pair.second;
        if (column->m_meshReadyForUpload) {
            column->uploadToGpu();
            uploadedCount++;
        }
    }

    for (auto& pair : m_chunks) {
        std::unique_ptr<ChunkColumn>& column = pair.second;


        if (column->m_isMeshDirty && !column->m_isGeneratingMesh && !column->m_meshReadyForUpload) {
            column->m_isGeneratingMesh = true;

            ChunkColumn* columnPtr = column.get();

            m_threadPool->enqueue([columnPtr]() {
                columnPtr->generateMesh();
            });
        }
    }
}

RaycastResult World::rayCast(const glm::vec3 &origin, const glm::vec3 &direction, float maxDist) {
    RaycastResult result;
    result.hit = false;


    int x = floor(origin.x);
    int y = floor(origin.y);
    int z = floor(origin.z);


    int stepX = (direction.x > 0) ? 1 : -1;
    int stepY = (direction.y > 0) ? 1 : -1;
    int stepZ = (direction.z > 0) ? 1 : -1;

    float tDeltaX = (direction.x != 0) ? std::abs(1.0f / direction.x) : 100000.0f;
    float tDeltaY = (direction.y != 0) ? std::abs(1.0f / direction.y) : 100000.0f;
    float tDeltaZ = (direction.z != 0) ? std::abs(1.0f / direction.z) : 100000.0f;


    float tMaxX = (direction.x > 0) ? (floor(origin.x) + 1 - origin.x) * tDeltaX : (origin.x - floor(origin.x)) * tDeltaX;
    float tMaxY = (direction.y > 0) ? (floor(origin.y) + 1 - origin.y) * tDeltaY : (origin.y - floor(origin.y)) * tDeltaY;
    float tMaxZ = (direction.z > 0) ? (floor(origin.z) + 1 - origin.z) * tDeltaZ : (origin.z - floor(origin.z)) * tDeltaZ;

    float distance = 0.0f;


    while (distance <= maxDist) {

        BlockType blockID = getBlockAt(glm::ivec3(x, y, z));
        if (blockID != BlockType::Air) {
            result.hit = true;
            result.blockPos = glm::ivec3(x, y, z);
            result.blockType = blockID;
            return result;
        }
        
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                x += stepX;
                distance = tMaxX;
                tMaxX += tDeltaX;
                result.faceNormal = glm::ivec3(-stepX, 0, 0);
            } else {
                z += stepZ;
                distance = tMaxZ;
                tMaxZ += tDeltaZ;
                result.faceNormal = glm::ivec3(0, 0, -stepZ);
            }
        } else {
            if (tMaxY < tMaxZ) {
                y += stepY;
                distance = tMaxY;
                tMaxY += tDeltaY;
                result.faceNormal = glm::ivec3(0, -stepY, 0);
            } else {
                z += stepZ;
                distance = tMaxZ;
                tMaxZ += tDeltaZ;
                result.faceNormal = glm::ivec3(0, 0, -stepZ);
            }
        }
    }

    return result;
}


void World::unloadFarChunks(int playerChunkX,int playerChunkZ) {
    const int unloadRadius = static_cast<int>(Config::chunkRadius) + 1;
    std::vector<glm::ivec2> toRemove;
    toRemove.reserve(32);

    std::lock_guard<std::mutex> lock(m_chunkMapMutex);

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


        if (it->second->m_isGeneratingMesh) {
            continue;
        }

        it->second->destroyGL();
        it->second->disconnectNeighbours();
        m_chunks.erase(it);
    }
}

const ChunkMap &World::getChunks()const {
    return m_chunks;
}
ChunkColumn* World::getChunkColumn(int chunkPosX,int chunkPosZ) {
    glm::ivec2 pos(chunkPosX, chunkPosZ);

    if (pos == m_lastAccessedPos) {
        return m_lastAccessedColumn;
    }

    auto chunkIt = m_chunks.find(pos);

    if (chunkIt != m_chunks.end()) {
        m_lastAccessedPos = pos;
        m_lastAccessedColumn = chunkIt->second.get();
        return m_lastAccessedColumn;
    }

    m_lastAccessedPos = pos;
    m_lastAccessedColumn = nullptr;
    return nullptr;
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