#include "ChunkColumn.hpp"

#include <FastNoiseLite.h>

#include "Config.hpp"
#include "glad/glad.h"
#include <iostream>
#include <World.hpp>

#include "WorldGenerator.hpp"


ChunkColumn::ChunkColumn(int x,int z,World *world) {

    m_nbrChunkColumnNX = nullptr;
    m_nbrChunkColumnNZ = nullptr;
    m_nbrChunkColumnPX = nullptr;
    m_nbrChunkColumnPZ = nullptr;

    m_posX = x;
    m_posZ = z;
    m_chunks.reserve(Config::chunkColumnHeight);
    for(int i=0;i < Config::chunkColumnHeight ;i++) {
        m_chunks.emplace_back(m_posX,i,m_posZ,world);
    }


    unsigned int offsetX = m_posX * Config::chunkSize;
    unsigned int  offsetZ = m_posZ * Config::chunkSize;
    for(unsigned int localX = 0; localX < Config::chunkSize; localX++) {
        for(unsigned int localZ = 0; localZ < Config::chunkSize; localZ++) {
            const int globalX = localX + offsetX;
            const int globalZ = localZ + offsetZ;


            const float noiseVal = world->getHeightNoiseVal(globalX,globalZ);
            const int terrainHeight = static_cast<int>((noiseVal / 2.0f) * Config::chunkMaxBlockHeight);

            float temp = world->getTemperatureNoiseVal(globalX,globalZ);
            float humiditiy = world->getHumidityNoiseVal(globalX,globalZ);
            BiomeType biome = WorldGenerator::getBiomeType(temp,humiditiy,terrainHeight);
            for(int globalY = 0; globalY < Config::chunkMaxBlockHeight; globalY++) {

                BlockType blockToSet;
                if (globalY > terrainHeight) {

                    if (globalY <= Config::SEA_LEVEL) {
                        blockToSet = BlockType::Water;
                    } else {

                        blockToSet = BlockType::Air;
                    }
                } else {

                    blockToSet = WorldGenerator::generateBlock(globalY,biome);
                }


                const int chunkY = globalY / Config::chunkSize;
                const int localY = globalY % Config::chunkSize;
                if( m_chunks[chunkY].getBlock(localX, localY, localZ)==BlockType::Air) {
                    m_chunks[chunkY].setBlock(blockToSet, localX, localY, localZ);
                }
            }

            float treeNoiseVal = world->getTreeNoiseVal(globalX,globalZ);
            WorldGenerator::spawnNature(this,treeNoiseVal,localX,terrainHeight,localZ,biome);

        }
    }

    m_solidMesh.reserve(Config::chunkSize * Config::chunkSize * 6);
    m_transparentMesh.reserve(Config::chunkSize * Config::chunkSize );
}
ChunkColumn::~ChunkColumn() {
    destroyGL();           // free VAO/VBO
    m_solidMesh.clear();
    m_solidMesh.shrink_to_fit();
    m_chunks.clear();
    m_chunks.shrink_to_fit();
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
BlockType ChunkColumn::getBlockAt(int chunkPosY, int x, int y, int z) {
    if (chunkPosY < 0 || chunkPosY >= m_chunks.size()) return BlockType::Air;
    return m_chunks[chunkPosY].getBlock(x, y, z);

}
void ChunkColumn::spawnTree(const float treeNoiseVal,int localX,int localZ,int terrainHeight) {

}

void ChunkColumn::generateMesh() {
    m_solidMesh.clear();
    m_solidMesh.shrink_to_fit();
    m_solidMesh.reserve(Config::chunkSize * Config::chunkSize * 6);

    m_transparentMesh.clear();
    m_transparentMesh.shrink_to_fit();
    m_transparentMesh.reserve(Config::chunkSize * Config::chunkSize );

    destroyGL();

    for(int i=0;i<16;i++) {
        Chunk &chunk = m_chunks[i];

        Chunk *chunkNy = i>0 ?&m_chunks[i-1]:nullptr;
        Chunk *chunkPy = i<15 ?&m_chunks[i+1]:nullptr;


        Chunk *chunkNx = m_nbrChunkColumnNX != nullptr ? m_nbrChunkColumnNX->getChunk(i) : nullptr;
        Chunk *chunkPx = m_nbrChunkColumnPX != nullptr ? m_nbrChunkColumnPX->getChunk(i) : nullptr;
        Chunk *chunkNz = m_nbrChunkColumnNZ != nullptr ? m_nbrChunkColumnNZ->getChunk(i) : nullptr;
        Chunk *chunkPz = m_nbrChunkColumnPZ != nullptr ? m_nbrChunkColumnPZ->getChunk(i) : nullptr;

        chunk.generateMesh(
            m_solidMesh,
            m_transparentMesh,
            chunkNx,
            chunkPx,
            chunkNy,
            chunkPy,
            chunkNz,
            chunkPz
            );

    }

    isMeshDirty = false;
    cpuMeshReady = true;
}
void ChunkColumn::uploadToGpu() {
    glGenVertexArrays(1, &m_solidVAO);
    glGenBuffers(1, &m_solidVBO);
    glBindVertexArray(m_solidVAO);


    glBindBuffer(GL_ARRAY_BUFFER, m_solidVBO);
    glBufferData(GL_ARRAY_BUFFER, m_solidMesh.size() * sizeof(Face), m_solidMesh.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, ao));

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
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));


     m_transparentMeshSize = m_transparentMesh.size();
     m_transparentMesh.clear();
     m_transparentMesh.shrink_to_fit();

    gpuMeshReady = true;
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
        m_nbrChunkColumnNX->isMeshDirty = true;
    }
    if(m_nbrChunkColumnPX) {
        m_nbrChunkColumnPX->setNeighbourNx(nullptr);
        m_nbrChunkColumnPX->isMeshDirty = true;
    }
    if(m_nbrChunkColumnPZ) {
        m_nbrChunkColumnPZ->setNeighbourNz(nullptr);
        m_nbrChunkColumnPZ->isMeshDirty = true;
    }
    if(m_nbrChunkColumnNZ) {
        m_nbrChunkColumnNZ->setNeighbourPz(nullptr);
        m_nbrChunkColumnNZ->isMeshDirty = true;
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