//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

uint32_t simple_hash(const uint8_t* data, size_t len) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 16777619u;
    }
    return hash;
}

void process_chunk(Chunk* chunk) {
    if (chunk->size == 0) return;
    
    uint32_t hash = simple_hash(chunk->data, chunk->size);
    
    for (size_t i = 0; i < chunk->size; i++) {
        chunk->data[i] ^= (hash >> ((i % 4) * 8)) & 0xFF;
        chunk->data[i] = (chunk->data[i] + i) & 0xFF;
    }
    
    for (size_t i = 0; i < chunk->size / 2; i++) {
        uint8_t temp = chunk->data[i];
        chunk->data[i] = chunk->data[chunk->size - 1 - i];
        chunk->data[chunk->size - 1 - i] = temp;
    }
}

int read_input(uint8_t* buffer, size_t max_size) {
    size_t total_read = 0;
    
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF) break;
        if (c < 0 || c > 255) continue;
        
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    
    return total_read > 0 ? 1 : 0;
}

void write_output(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        putchar(data[i]);
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    
    if (!read_input(input_buffer, MAX_INPUT_SIZE)) {
        return 1;
    }
    
    size_t input_size = 0;
    while (input_size < MAX_INPUT_SIZE && input_buffer[input_size] != 0) {
        input_size++;
    }
    
    if (input_size == 0) {
        return 1;
    }
    
    Chunk chunks[256];
    size_t num_chunks = 0;
    size_t remaining = input_size;
    
    for (size_t i = 0; i < 256 && remaining > 0; i++) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        memcpy(chunks[i].data, input_buffer + i * CHUNK_SIZE, chunk_size);
        chunks[i].size = chunk_size;
        num_chunks++;
        remaining -= chunk_size;
    }
    
    for (size_t i = 0; i < num_chunks; i++) {
        process_chunk(&chunks[i]);
    }
    
    for (size_t i = 0; i < num_chunks; i++) {
        write_output(chunks[i].data, chunks[i].size);
    }
    
    return 0;
}