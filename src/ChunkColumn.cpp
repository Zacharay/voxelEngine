#include "ChunkColumn.hpp"
#include "Config.hpp"
#include <iostream>

ChunkColumn::ChunkColumn(const std::array<std::array<unsigned int,Config::noiseWidth>,Config::noiseWidth> &noiseData,int x,int z) {

    m_posX = x;
    m_posZ = z;
    for(int i=0;i < Config::chunkColumnHeight ;i++) {
        m_chunks.emplace_back(m_posX,i,m_posZ);
    }



    //todo calculate this offsets properly
    int offsetX = m_posX * Config::chunkSize +256;
    int offsetZ = m_posZ * Config::chunkSize +256;
    for(int x=0;x<Config::chunkSize;x++)
        for(int z=0;z<Config::chunkSize;z++) {


            const int noiseVal = noiseData[offsetX + x][offsetZ+z];


            for(int y=0;y<=noiseVal;y++) {

                const int chunkYPos = y/16;


                BlockType block;
                if(y<60) {
                    block = BlockType::Stone;
                }
                else if(y > 60 && y < 75) {
                    block= BlockType::Sand;
                }
                else {
                    block = BlockType::Grass;
                }



                m_chunks[chunkYPos].setBlock(block,x,y - chunkYPos*Config::chunkSize,z);




            }


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