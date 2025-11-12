//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} ChunkArray;

void init_chunk_array(ChunkArray *array) {
    array->chunks = NULL;
    array->count = 0;
    array->capacity = 0;
}

int add_chunk(ChunkArray *array, const uint8_t *data, size_t size) {
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity == 0 ? 4 : array->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(Chunk)) {
            return 0;
        }
        Chunk *new_chunks = realloc(array->chunks, new_capacity * sizeof(Chunk));
        if (!new_chunks) {
            return 0;
        }
        array->chunks = new_chunks;
        array->capacity = new_capacity;
    }
    
    if (size > CHUNK_SIZE) {
        return 0;
    }
    
    Chunk *chunk = &array->chunks[array->count];
    memcpy(chunk->data, data, size);
    chunk->size = size;
    array->count++;
    return 1;
}

void free_chunk_array(ChunkArray *array) {
    free(array->chunks);
    array->chunks = NULL;
    array->count = 0;
    array->capacity = 0;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t input_pos = 0;
    
    while (input_pos < input_size && output_pos < output_size - 2) {
        uint8_t current = input[input_pos];
        size_t count = 1;
        
        while (input_pos + count < input_size && count < 255 && input[input_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[output_pos++] = 0xFF;
            if (output_pos >= output_size) break;
            output[output_pos++] = current;
            if (output_pos >= output_size) break;
            output[output_pos++] = (uint8_t)count;
            if (output_pos >= output_size) break;
        } else {
            for (size_t i = 0; i < count && output_pos < output_size; i++) {
                output[output_pos++] = current;
            }
        }
        
        input_pos += count;
    }
    
    return output_pos;
}

int process_compression(ChunkArray *input, ChunkArray *output) {
    for (size_t i = 0; i < input->count; i++) {
        Chunk *in_chunk = &input->chunks[i];
        uint8_t compressed[CHUNK_SIZE * 2];
        
        size_t compressed_size = simple_compress(in_chunk->data, in_chunk->size, compressed, sizeof(compressed));
        if (compressed_size == 0 && in_chunk->size > 0) {
            return 0;
        }
        
        if (!add_chunk(output, compressed, compressed_size)) {
            return 0;
        }
    }
    return 1;
}

void generate_test_data(ChunkArray *array) {
    uint8_t test_data[CHUNK_SIZE];
    
    for (int pattern = 0; pattern < 8; pattern++) {
        size_t data_size = CHUNK_SIZE;
        
        switch (pattern % 4) {
            case 0:
                memset(test_data, 'A' + (pattern % 26), data_size);
                break;
            case 1:
                for (size_t i = 0; i < data_size; i++) {
                    test_data[i] = (uint8_t)(i % 256);
                }
                break;
            case 2:
                for (size_t i = 0; i < data_size; i++) {
                    test_data[i] = (uint8_t)((i * 7) % 256);
                }
                break;
            case 3:
                memset(test_data, 0, data_size);
                for (size_t i = 0; i < data_size; i += 16) {
                    test_data[i] = 0xFF;
                }
                break;
        }
        
        if (!add_chunk(array, test_data, data_size)) {
            return;
        }
    }
}

int main(void) {
    ChunkArray input_data;
    ChunkArray compressed_data;
    
    init_chunk_array(&input_data);
    init_chunk_array(&compressed_data);
    
    generate_test_data(&input_data);
    
    if (input_data.count == 0) {
        printf("Failed to generate test data\n");
        free_chunk_array(&input_data);
        return 1;
    }
    
    size_t total_input = 0;
    for (size_t i = 0; i < input_data.count; i++) {
        total