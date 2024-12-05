#include <stdio.h>

#include "debug.h"
#include "value.h"

//temp function for debugging delete later
// void printLineArray(LineArray* array){
//     for (int i =0; i < array->count; i++){
//         int line = array->lines[i].line;
//         int start = array->lines[i].start_offset;
//         int end = array->lines[i].end_offset;
//     }
// }

void disassembleChunk(Chunk* chunk, const char* name){
    // printLineArray(&chunk->line_arr);
    printf("==%s==\n", name);

    for (int offset = 0; offset < chunk->count;){
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset){
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset){
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

int getLine(LineArray* array, int offset){
    if (offset < 0){
        return -1;
    }
    for (int i = 0; i < array->count; i++){
        if (array->lines[i].end_offset >= offset){
            return array->lines[i].line;
        }
    }
    return -1;
}

int disassembleInstruction(Chunk* chunk, int offset){
    printf("%04d ", offset);

    int curr_line = getLine(&chunk->line_arr, offset);
    int prev_line = getLine(&chunk->line_arr, offset - 1); 
    if (offset > 0 && curr_line == prev_line){
        printf("   | ");
    }
    else{
        printf("%4d ", curr_line);
    }

    uint8_t instruction = chunk->code[offset];
    switch(instruction){
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("unknown opcode %d\n", instruction);
            return offset + 1;
    }
}