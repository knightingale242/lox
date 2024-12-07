#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

// int main(int argc, const char* argv[]){
//     initVM();
//     Chunk chunk;
//     initChunk(&chunk);
//     //validate new constant long behavior will keep for now
//     // for (int i = 1; i < 300; i++){
//     //     writeConstant(&chunk, i, i);
//     // }
//     writeConstant(&chunk, 1.2, 123);
//     writeConstant(&chunk, 3.4, 123);
//     writeChunk(&chunk, OP_ADD, 123);
//     writeConstant(&chunk, 5.6, 123);
//     writeChunk(&chunk, OP_DIVIDE, 123);
//     writeChunk(&chunk, OP_NEGATE, 123);
//     writeChunk(&chunk, OP_RETURN, 123);
//     // disassembleChunk(&chunk, "test chunk");
//     interpret(&chunk);
//     freeVM();
//     freeChunk(&chunk);

//     return 0;
// }

static void repl(){
    char line[1024];
    for (;;){
        printf("> ");
        if(fgets(line, sizeof(line), stdin)){
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static char* readFile(const char* path){
    FILE* file = fopen(path, "rb");
    if (file == NULL){
        fprintf(stderr, "Error opening the file \"%s\".\n", path);
        exit(74);
    }
    
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path){
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]){
    initVM();
    if (argc == 1){
        repl();
    }
    else if(argc == 2){
        runFile(argv[1]);
    }
    else{
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }
    freeVM();
    return 0;
}