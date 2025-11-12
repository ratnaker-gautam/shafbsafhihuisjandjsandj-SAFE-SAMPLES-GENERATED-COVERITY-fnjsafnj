//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    unsigned char data[CHUNK_SIZE];
    size_t size;
} Chunk;

int read_input(Chunk *chunks, size_t *chunk_count) {
    size_t total_size = 0;
    *chunk_count = 0;
    
    while (total_size < MAX_INPUT_SIZE) {
        Chunk *current = &chunks[*chunk_count];
        size_t bytes_read = fread(current->data, 1, CHUNK_SIZE, stdin);
        
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            if (ferror(stdin)) return -1;
        }
        
        current->size = bytes_read;
        total_size += bytes_read;
        (*chunk_count)++;
        
        if (bytes_read < CHUNK_SIZE) break;
    }
    
    if (total_size >= MAX_INPUT_SIZE) {
        return -1;
    }
    
    return 0;
}

void simple_compress(const Chunk *chunks, size_t chunk_count) {
    for (size_t i = 0; i < chunk_count; i++) {
        const Chunk *chunk = &chunks[i];
        
        for (size_t j = 0; j < chunk->size; j++) {
            unsigned char current = chunk->data[j];
            unsigned char count = 1;
            
            while (j + 1 < chunk->size && chunk->data[j + 1] == current && count < UCHAR_MAX) {
                count++;
                j++;
            }
            
            if (fwrite(&count, 1, 1, stdout) != 1) exit(1);
            if (fwrite(&current, 1, 1, stdout) != 1) exit(1);
        }
    }
}

int main(void) {
    Chunk chunks[MAX_INPUT_SIZE / CHUNK_SIZE + 1];
    size_t chunk_count;
    
    if (read_input(chunks, &chunk_count) != 0) {
        fprintf(stderr, "Error: Input too large or read error\n");
        return 1;
    }
    
    if (chunk_count == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    simple_compress(chunks, chunk_count);
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error: Output flush failed\n");
        return 1;
    }
    
    return 0;
}