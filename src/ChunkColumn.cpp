#include "ChunkColumn.hpp"

#include <FastNoiseLite.h>

#include "Config.hpp"
#include "glad/glad.h"
#include <iostream>
#include "WorldGenerator.hpp"


ChunkColumn::ChunkColumn(FastNoiseLite& m_noise,int x,int z) {

    m_posX = x;
    m_posZ = z;
    m_chunks.reserve(Config::chunkColumnHeight);
    for(int i=0;i < Config::chunkColumnHeight ;i++) {
        m_chunks.emplace_back(m_posX,i,m_posZ);
    }

    int offsetX = m_posX * Config::chunkSize;
    int offsetZ = m_posZ * Config::chunkSize;
    for(int x=0;x<Config::chunkSize;x++)
        for(int z=0;z<Config::chunkSize;z++) {

            const float noiseVal = m_noise.GetNoise(static_cast<float>(x+offsetX), static_cast<float>(z+offsetZ)) + 1.0f;

            const unsigned int blockHeight = (noiseVal / 2.0f) * Config::chunkMaxBlockHeight;


            for(int y=0;y<=blockHeight;y++) {

                const int chunkYPos = y/Config::chunkSize;

                m_chunks[chunkYPos].setBlock(WorldGenerator::generateBlock(y),x,y - chunkYPos*Config::chunkSize,z);

            }


        }

    m_mesh.reserve(Config::chunkSize * Config::chunkSize * 6);
}
ChunkColumn::~ChunkColumn() {
    destroyGL();           // free VAO/VBO
    m_mesh.clear();
    m_mesh.shrink_to_fit();
    m_chunks.clear();
    m_chunks.shrink_to_fit();
}
void ChunkColumn::destroyGL() {
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}
void ChunkColumn::generateMesh() {
    m_mesh.clear();
    m_mesh.shrink_to_fit();
    m_mesh.reserve(Config::chunkSize * Config::chunkSize * 6);

    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }

    for(int i=0;i<16;i++) {
        Chunk &chunk = m_chunks[i];

        Chunk *chunkNy = i>0 ?&m_chunks[i-1]:nullptr;
        Chunk *chunkPy = i<15 ?&m_chunks[i+1]:nullptr;


        Chunk *chunkNx = m_nbrChunkColumnNX != nullptr ? m_nbrChunkColumnNX->getChunk(i) : nullptr;
        Chunk *chunkPx = m_nbrChunkColumnPX != nullptr ? m_nbrChunkColumnPX->getChunk(i) : nullptr;
        Chunk *chunkNz = m_nbrChunkColumnNZ != nullptr ? m_nbrChunkColumnNZ->getChunk(i) : nullptr;
        Chunk *chunkPz = m_nbrChunkColumnPZ != nullptr ? m_nbrChunkColumnPZ->getChunk(i) : nullptr;

        chunk.generateMesh(
            m_mesh,
            chunkNx,
            chunkPx,
            chunkNy,
            chunkPy,
            chunkNz,
            chunkPz
            );

    }
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);


    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_mesh.size() * sizeof(Face), m_mesh.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    isMeshDirty = false;
    m_meshSize = m_mesh.size();
    m_mesh.clear();
    m_mesh.shrink_to_fit();
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
void ChunkColumn::bindMesh()const {
    glBindVertexArray(m_VAO);
}
const std::vector<Face>& ChunkColumn::getMesh()const {
    return m_mesh;
}
Chunk * ChunkColumn::getChunk(int height) {
    return &m_chunks[height];
}