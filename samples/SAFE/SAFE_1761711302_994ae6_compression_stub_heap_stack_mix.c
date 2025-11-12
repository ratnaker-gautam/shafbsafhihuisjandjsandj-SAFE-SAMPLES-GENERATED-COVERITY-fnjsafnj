//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} CompressedData;

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

int init_compressed_data(CompressedData *cd, size_t initial_capacity) {
    if (cd == NULL || initial_capacity == 0) return 0;
    cd->chunks = malloc(initial_capacity * sizeof(Chunk));
    if (cd->chunks == NULL) return 0;
    cd->count = 0;
    cd->capacity = initial_capacity;
    return 1;
}

void free_compressed_data(CompressedData *cd) {
    if (cd != NULL) {
        free(cd->chunks);
        cd->chunks = NULL;
        cd->count = 0;
        cd->capacity = 0;
    }
}

int add_chunk(CompressedData *cd, const uint8_t *data, size_t size) {
    if (cd == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    if (cd->count >= cd->capacity) {
        size_t new_capacity = cd->capacity * 2;
        if (new_capacity <= cd->capacity) return 0;
        Chunk *new_chunks = realloc(cd->chunks, new_capacity * sizeof(Chunk));
        if (new_chunks == NULL) return 0;
        cd->chunks = new_chunks;
        cd->capacity = new_capacity;
    }
    memcpy(cd->chunks[cd->count].data, data, size);
    cd->chunks[cd->count].size = size;
    cd->count++;
    return 1;
}

void simple_compress(const char *input, size_t input_len, CompressedData *output) {
    if (!validate_input(input, input_len) || output == NULL) return;
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = (input_len - processed) < CHUNK_SIZE ? (input_len - processed) : CHUNK_SIZE;
        if (!add_chunk(output, (const uint8_t *)(input + processed), chunk_size)) {
            break;
        }
        processed += chunk_size;
    }
}

void print_compressed_data(const CompressedData *cd) {
    if (cd == NULL) return;
    printf("Compressed data (%zu chunks):\n", cd->count);
    for (size_t i = 0; i < cd->count; i++) {
        printf("Chunk %zu (size %zu): ", i, cd->chunks[i].size);
        for (size_t j = 0; j < cd->chunks[i].size; j++) {
            printf("%02X ", cd->chunks[i].data[j]);
        }
        printf("\n");
    }
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    CompressedData compressed;
    if (!init_compressed_data(&compressed, 4)) {
        fprintf(stderr, "Failed to initialize compression data\n");
        return 1;
    }
    
    simple_compress(input_buffer, input_len, &compressed);
    
    if (compressed.count == 0) {
        fprintf(stderr, "Compression failed\n");
        free_compressed_data(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed to %zu chunks\n", compressed.count);
    print_compressed_data(&compressed);
    
    free_compressed_data(&compressed);
    return 0;
}