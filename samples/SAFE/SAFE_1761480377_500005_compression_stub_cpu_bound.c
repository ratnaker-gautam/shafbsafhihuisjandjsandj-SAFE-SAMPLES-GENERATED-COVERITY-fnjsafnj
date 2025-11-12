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

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} ChunkList;

static int chunk_list_init(ChunkList *list) {
    list->capacity = 16;
    list->count = 0;
    list->chunks = malloc(list->capacity * sizeof(Chunk));
    return list->chunks != NULL;
}

static void chunk_list_free(ChunkList *list) {
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

static int chunk_list_append(ChunkList *list, const uint8_t *data, size_t size) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, new_cap * sizeof(Chunk));
        if (!new_chunks) return 0;
        list->chunks = new_chunks;
        list->capacity = new_cap;
    }
    
    if (size > CHUNK_SIZE) return 0;
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

static size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
        if (output_idx > MAX_INPUT_SIZE) break;
    }
    
    return output_idx;
}

static int process_compression(ChunkList *input, ChunkList *output) {
    if (!chunk_list_init(output)) return 0;
    
    for (size_t i = 0; i < input->count; i++) {
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_size = simple_compress(input->chunks[i].data, input->chunks[i].size, compressed);
        
        if (compressed_size == 0 || compressed_size > sizeof(compressed)) {
            chunk_list_free(output);
            return 0;
        }
        
        if (!chunk_list_append(output, compressed, compressed_size)) {
            chunk_list_free(output);
            return 0;
        }
    }
    
    return 1;
}

static size_t read_input_data(uint8_t *buffer) {
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF) break;
        buffer[total_read++] = (uint8_t)c;
    }
    
    return total_read;
}

static void write_output_data(const ChunkList *output) {
    for (size_t i = 0; i < output->count; i++) {
        fwrite(output->chunks[i].data, 1, output->chunks[i].size, stdout);
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = read_input_data(input_buffer);
    
    if (input_size == 0) {
        return 1;
    }
    
    ChunkList input_chunks, output_chunks;
    
    if (!chunk_list_init(&input_chunks)) {
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t chunk_size = input_size - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        if (!chunk_list_append(&input_chunks, input_buffer + processed, chunk_size)) {
            chunk_list_free(&input_chunks);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    int result = process_compression(&input_chunks, &output_chunks);
    
    if (result) {
        write_output_data(&output_chunks);
        chunk_list_free(&output_chunks);
    }
    
    chunk_list_free(&input_chunks);
    return result ? 0 : 1;
}