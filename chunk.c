#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void initLineArray(LineArray* array){
    array->count = 0;
    array->capacity = 0;
    array->lines = NULL;
}
/*
this function makes the assumption that all code is generated and
emitted sequentially hence why i only check the latest entry in the
line array to see if the line matches, instructions from previous lines
should not be emitted after instructions from lines after them

the reason endoffset gets update with the new_offset if the line matches 
is based on assumptions about how byte code is parsed(byte by byte) so it
is give based on this that the new_offset will be greater that the current
end_offset

each entry in the line array acts as a blip of information for that line and
the instructions inside of it. it is an INVARIANT that all instructions between
starting at the start_offset and the end_offset of a LineInfo entry belong to that
line and this is how lines of an instruction will be retrieved when queried for. so
if a new entry is found for the same line then it is known that it is a later offset
than what is currently set and we can extend the window of bytecode that belong to this
line to include this instruction :)
*/
void writeLineArray(LineArray* array, int line, int new_offset){
    if (array->count > 0 && array->lines[array->count - 1].line == line) {
        array->lines[array->count - 1].end_offset = new_offset;
    }
    else{
        if (array->capacity < array->count + 1){
            int old_capacity = array->capacity;
            array->capacity = GROW_CAPACITY(old_capacity);
            array->lines = GROW_ARRAY(LineInfo, array->lines, old_capacity, array->capacity);
        }
        array->lines[array->count].line = line;
        array->lines[array->count].start_offset = new_offset;
        array->lines[array->count].end_offset = new_offset;

        array->count++;
    }
}

void freeLineArray(LineArray* array){
    FREE_ARRAY(LineInfo, array->lines, array->capacity);
    initLineArray(array);
}

void initChunk(Chunk* chunk){
    // printf("initializing the chunk :)\n");
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initLineArray(&chunk->line_arr);
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk){
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->line_arr);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line){
    if (chunk->capacity < chunk->count + 1){
        int old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    }

    writeLineArray(&chunk->line_arr, line, chunk->count);
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value){
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

/*
just a note on some c things that i learned because of a bug i
fixed in this code, i was initially passing &chunk into all the
function calls in here that took chunk and was getting a seg error

after some log statements and thinking i realized that it was a gap
in my knowledge on pointers, i had already passed &chunk into the invocation
of this function so i currently have chunk* and if i invoke other functions
that require the same thing i can just pass chunk and not not &chunk because that
would be passing in chunk** not chunk* not sure why it did not complain but remember
this as things get more complex in this code :)
*/

/*
Context: before implementing this we would be limited to having 255 constants
available to look up because the code only supported adding adding indexes that
could be represented with one byte. now it can hold up to 16,777,215 now that the
index is represented with three bytes

complexity is added because the switch is done once we hit the limit and start using new
opcode and logic will have to be added to handle dissassembling this in the debug file to
accomodate the new code however it does save space not sure if it is worth it

255 * 2 = 510 bytes or i could just make all constants represented by longs and keep it simple
the additional complexity is not really on the critical path though. will sleep on it :)
*/
void writeConstant(Chunk* chunk, Value value, int line){
    int index = addConstant(chunk, value);
    if (index <= 255){
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, index, line);
    }
    else {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, (index >> 16) & 0xFF, line);
        writeChunk(chunk, (index >> 8) & 0xFF, line);
        writeChunk(chunk, index & 0xFF, line);
    }
}
