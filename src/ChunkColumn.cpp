#include "ChunkColumn.hpp"

#include <iostream>

ChunkColumn::ChunkColumn(int height,int x,int z) {

    m_posX = x;
    m_posZ = z;
    for(int i=0;i<16;i++) {
        BlockType block;

        if(i > height) {
            block = BlockType::Air;
        }
        else {
            if(i < 4 ) {
                block = BlockType::Stone;
            }
            else {
                block = BlockType::Grass;
            }
        }
        m_chunks.emplace_back(m_posX,i,m_posZ,block);
    }




}

void ChunkColumn::generateMesh() {
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

}
void ChunkColumn::setNeighbouringChunks(ChunkColumn* chunkNx,ChunkColumn* chunkPx,ChunkColumn* chunkNz,ChunkColumn* chunkPz) {
    m_nbrChunkColumnNX = chunkNx;;
    m_nbrChunkColumnPX = chunkPx;
    m_nbrChunkColumnPZ = chunkPz;
    m_nbrChunkColumnNZ = chunkNz;
}

const std::vector<Face>& ChunkColumn::getMesh()const {
    return m_mesh;
}
Chunk * ChunkColumn::getChunk(int height) {
    return &m_chunks[height];

}