#include "Chunk.hpp"

Chunk::Chunk() {

    for(int i=0;i<16;i++)
        for(int j=0;j<16;j++)
            for(int k=0;k<16;k++) {
                blocks[i][j][k] = 0;
            }

}
