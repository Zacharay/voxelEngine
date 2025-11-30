#include "ChunkColumn.hpp"

#include <FastNoiseLite.h>

#include "Config.hpp"
#include "glad/glad.h"
#include <iostream>
#include <World.hpp>

#include "GuiManager.hpp"
#include "WorldGenerator.hpp"


ChunkColumn::ChunkColumn(int x,int z,World& world):m_posX(x),m_posZ(z),m_world(world)
{
    for(int y=0;y < Config::chunkColumnHeight ;y++) {
        m_chunks[y].init(m_posX,y,m_posZ,world);
    }

    generateTerrain();
}
ChunkColumn::~ChunkColumn() {
    destroyGL();           // free VAO/VBO
}
void ChunkColumn::destroyGL() {
    if (m_solidVBO) {
        glDeleteBuffers(1, &m_solidVBO);
        m_solidVBO = 0;
    }
    if (m_solidVAO) {
        glDeleteVertexArrays(1, &m_solidVAO);
        m_solidVAO = 0;
    }

    if (m_transparentVBO) {
        glDeleteBuffers(1, &m_transparentVBO);
        m_transparentVBO = 0;
    }
    if (m_transparentVAO) {
        glDeleteVertexArrays(1, &m_transparentVAO);
        m_transparentVAO = 0;
    }
}
BlockType ChunkColumn::getBlockAt(int localX,int worldY,int localZ)const {
    if(worldY < 0 || worldY >= Config::chunkMaxBlockHeight)return BlockType::Air;

    const int chunkYIndex = worldY  >> Config::chunkSizeShift;
    const int localY = worldY & Config::chunkSizeMask;

    return m_chunks[chunkYIndex].getBlock(localX,localY,localZ);

}
void ChunkColumn::setBlockAt(int localX, int worldY, int localZ,BlockType block)  {
    if(worldY < 0 || worldY >= Config::chunkMaxBlockHeight) {
        std::cerr << "setBlockAt: worldY out of bounds" << std::endl;
    }

    const int chunkYIndex = worldY  >> Config::chunkSizeShift;
    const int localY = worldY & Config::chunkSizeMask;

    m_chunks[chunkYIndex].setBlock(localX,localY,localZ,block);

    m_isMeshDirty = true;
}
void ChunkColumn::generateTerrain() {

    const int  offsetX = m_posX * static_cast<int>(Config::chunkSize);
    const int  offsetZ = m_posZ * static_cast<int>(Config::chunkSize);

    constexpr int TERRAIN_BASE_HEIGHT = Config::SEA_LEVEL - 40;
    constexpr int TERRAIN_AMPLITUDE = 100;

    for(int localX = 0; localX < Config::chunkSize; localX++) {
        for(int localZ = 0; localZ < Config::chunkSize; localZ++) {
            const int globalX = localX + offsetX;
            const int globalZ = localZ + offsetZ;


            const float heightNoiseVal = m_world.getHeightNoiseVal(globalX,globalZ);
            const float temp = m_world.getTemperatureNoiseVal(globalX,globalZ);
            const float humidity = m_world.getHumidityNoiseVal(globalX,globalZ);


            const float normalizedNoise = (heightNoiseVal) / 2.0f;


            const int terrainHeight = static_cast<int>(TERRAIN_BASE_HEIGHT + (normalizedNoise * TERRAIN_AMPLITUDE));


            BiomeType biome = WorldGenerator::getBiomeType(temp,humidity,terrainHeight);


            for(int globalY = 0; globalY < Config::chunkMaxBlockHeight; globalY++) {

                BlockType blockToSet;
                if (globalY > terrainHeight) {

                    if (globalY <= Config::SEA_LEVEL) {
                        blockToSet = BlockType::Water;
                    } else {
                        break;
                    }
                } else {
                    blockToSet = WorldGenerator::generateBlock(globalY,biome);
                }


                setBlockAt(localX,globalY,localZ,blockToSet);

            }

            const float treeNoiseVal = m_world.getTreeNoiseVal(globalX,globalZ);
            WorldGenerator::spawnNature(*this,treeNoiseVal,localX,terrainHeight,localZ,biome);

        }
    }
}
void ChunkColumn::generateMesh() {

    std::vector<Face> tempSolidMesh;
    std::vector<Face> tempTransparentMesh;

    tempSolidMesh.reserve(Config::chunkSize * Config::chunkSize * Config::chunkSize);
    tempTransparentMesh.reserve(Config::chunkSize * Config::chunkSize);

    for(int i=0;i<Config::chunkSize;i++) {

        Chunk &chunk = m_chunks[i];
        if(chunk.isChunkEmpty())continue;

        ChunkNeighbors neighbors;

        neighbors.bottom = i>0 ? &m_chunks[i-1]:nullptr;
        neighbors.top    = i<Config::chunkSize -1 ? &m_chunks[i+1]:nullptr;
        neighbors.left   = m_nbrChunkColumnNX != nullptr ? m_nbrChunkColumnNX->getChunk(i) : nullptr;
        neighbors.right  = m_nbrChunkColumnPX != nullptr ? m_nbrChunkColumnPX->getChunk(i) : nullptr;
        neighbors.back   = m_nbrChunkColumnNZ != nullptr ? m_nbrChunkColumnNZ->getChunk(i) : nullptr;
        neighbors.front  = m_nbrChunkColumnPZ != nullptr ? m_nbrChunkColumnPZ->getChunk(i) : nullptr;

        chunk.generateMesh(tempSolidMesh,tempTransparentMesh,neighbors);
    }

    // now lock the mutex to safely swap the generated data
    {
        std::lock_guard<std::mutex> lock(m_meshDataMutex);
        m_solidMesh = std::move(tempSolidMesh);
        m_transparentMesh = std::move(tempTransparentMesh);
    }


    m_meshReadyForUpload = true;
    m_isGeneratingMesh = false;
    m_isMeshDirty = false;
}
void ChunkColumn::uploadToGpu() {

    destroyGL();

    std::lock_guard<std::mutex> lock(m_meshDataMutex);

    glGenVertexArrays(1, &m_solidVAO);
    glGenBuffers(1, &m_solidVBO);
    glBindVertexArray(m_solidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_solidVBO);
    glBufferData(GL_ARRAY_BUFFER, m_solidMesh.size() * sizeof(Face), m_solidMesh.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 4, nullptr);



    m_solidMeshSize = m_solidMesh.size();
    m_solidMesh.clear();
    m_solidMesh.shrink_to_fit();

     //transparentMesh
     glGenVertexArrays(1, &m_transparentVAO);
     glGenBuffers(1, &m_transparentVBO);
     glBindVertexArray(m_transparentVAO);


     glBindBuffer(GL_ARRAY_BUFFER, m_transparentVBO);
     glBufferData(GL_ARRAY_BUFFER, m_transparentMesh.size() * sizeof(Face), m_transparentMesh.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 4, nullptr);



     m_transparentMeshSize = m_transparentMesh.size();
     m_transparentMesh.clear();
     m_transparentMesh.shrink_to_fit();

    m_meshReadyForUpload = false;
}

void ChunkColumn::setNeighbouringChunks(ChunkColumn* chunkNx,ChunkColumn* chunkPx,ChunkColumn* chunkNz,ChunkColumn* chunkPz) {
    m_nbrChunkColumnNX = chunkNx;;
    m_nbrChunkColumnPX = chunkPx;
    m_nbrChunkColumnPZ = chunkPz;
    m_nbrChunkColumnNZ = chunkNz;
}
void ChunkColumn::disconnectNeighbours() {
    if(m_nbrChunkColumnNX) {
        m_nbrChunkColumnNX->setNeighbourPx(nullptr);
        m_nbrChunkColumnNX->setMeshDirty(true);
    }
    if(m_nbrChunkColumnPX) {
        m_nbrChunkColumnPX->setNeighbourNx(nullptr);
        m_nbrChunkColumnPX->setMeshDirty(true);
    }
    if(m_nbrChunkColumnPZ) {
        m_nbrChunkColumnPZ->setNeighbourNz(nullptr);
        m_nbrChunkColumnPZ->setMeshDirty(true);
    }
    if(m_nbrChunkColumnNZ) {
        m_nbrChunkColumnNZ->setNeighbourPz(nullptr);
        m_nbrChunkColumnNZ->setMeshDirty(true);
    }

    m_nbrChunkColumnNX = nullptr;
    m_nbrChunkColumnPX = nullptr;
    m_nbrChunkColumnPZ = nullptr;
    m_nbrChunkColumnNZ = nullptr;
}
void ChunkColumn::bindSolidMesh()const {
    glBindVertexArray(m_solidVAO);
}
void ChunkColumn::bindTransparentMesh()const {
    glBindVertexArray(m_transparentVAO);
}
const std::vector<Face>& ChunkColumn::getMesh()const {
    return m_solidMesh;
}
Chunk * ChunkColumn::getChunk(int height) {
    return &m_chunks[height];
}
