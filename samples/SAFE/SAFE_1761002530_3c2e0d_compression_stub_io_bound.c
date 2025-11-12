//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
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

int read_input(Chunk *chunks, size_t *num_chunks) {
    size_t total_size = 0;
    *num_chunks = 0;
    
    while (total_size < MAX_INPUT_SIZE) {
        Chunk *current = &chunks[*num_chunks];
        size_t bytes_read = fread(current->data, 1, CHUNK_SIZE, stdin);
        
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            if (ferror(stdin)) return -1;
        }
        
        current->size = bytes_read;
        total_size += bytes_read;
        (*num_chunks)++;
        
        if (bytes_read < CHUNK_SIZE) break;
    }
    
    if (total_size == 0) return -1;
    return 0;
}

void simple_compress(const Chunk *chunks, size_t num_chunks) {
    for (size_t i = 0; i < num_chunks; i++) {
        const Chunk *chunk = &chunks[i];
        size_t pos = 0;
        
        while (pos < chunk->size) {
            unsigned char current = chunk->data[pos];
            size_t count = 1;
            
            while (pos + count < chunk->size && 
                   chunk->data[pos + count] == current && 
                   count < UCHAR_MAX) {
                count++;
            }
            
            if (count > 3 || current == 0xFF) {
                fputc(0xFF, stdout);
                fputc(current, stdout);
                fputc((unsigned char)count, stdout);
            } else {
                for (size_t j = 0; j < count; j++) {
                    fputc(current, stdout);
                }
            }
            
            pos += count;
        }
    }
}

int main(void) {
    Chunk chunks[MAX_INPUT_SIZE / CHUNK_SIZE + 1];
    size_t num_chunks;
    
    if (read_input(chunks, &num_chunks) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (num_chunks == 0) {
        fprintf(stderr, "No input data\n");
        return 1;
    }
    
    simple_compress(chunks, num_chunks);
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error writing output\n");
        return 1;
    }
    
    return 0;
}