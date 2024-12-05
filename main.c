#include "common.h"
#include "chunk.h"
#include "debug.h"
#include <stdio.h>

int main(int argc, const char* argv[]){
    Chunk chunk;
    initChunk(&chunk);
    printf("here before writing the constant\n");
    writeConstant(&chunk, 1.2, 123);

    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

    return 0;
}