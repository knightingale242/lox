#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef Value Value;

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_GREATER,
    OP_EQUAL,
    OP_LESS,
    OP_CONSTANT_LONG,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    int line;
    int start_offset;
    int end_offset;
} LineInfo;

typedef struct {
    int capacity;
    int count;
    LineInfo* lines;
} LineArray;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
    LineArray line_arr;
} Chunk;

void initLineArray(LineArray* array);
void writeLineArray(LineArray* array, int line, int offset);
void freeLineArray(LineArray* array);

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);

void writeConstant(Chunk* chunk, Value value, int line);

#endif