//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} ChunkArray;

int init_chunk_array(ChunkArray *arr, size_t initial_capacity) {
    if (arr == NULL || initial_capacity == 0) return 0;
    arr->chunks = malloc(initial_capacity * sizeof(Chunk));
    if (arr->chunks == NULL) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void free_chunk_array(ChunkArray *arr) {
    if (arr != NULL) {
        free(arr->chunks);
        arr->chunks = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

int add_chunk(ChunkArray *arr, const uint8_t *data, size_t size) {
    if (arr == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity <= arr->capacity) return 0;
        Chunk *new_chunks = realloc(arr->chunks, new_capacity * sizeof(Chunk));
        if (new_chunks == NULL) return 0;
        arr->chunks = new_chunks;
        arr->capacity = new_capacity;
    }
    memcpy(arr->chunks[arr->count].data, data, size);
    arr->chunks[arr->count].size = size;
    arr->count++;
    return 1;
}

void compress_chunk(Chunk *chunk) {
    if (chunk == NULL || chunk->size == 0) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < chunk->size; ) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        while (i + count < chunk->size && chunk->data[i + count] == current && count < 255) {
            count++;
        }
        if (count > 2) {
            if (write_pos + 2 > CHUNK_SIZE) break;
            chunk->data[write_pos++] = current;
            chunk->data[write_pos++] = (uint8_t)count;
            i += count;
        } else {
            if (write_pos + count > CHUNK_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                chunk->data[write_pos++] = current;
            }
            i += count;
        }
    }
    chunk->size = write_pos;
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    ChunkArray chunks;
    if (!init_chunk_array(&chunks, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = (input_len - processed) < CHUNK_SIZE ? (input_len - processed) : CHUNK_SIZE;
        if (!add_chunk(&chunks, (const uint8_t*)(input + processed), chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_array(&chunks);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Original data: %s\n", input);
    printf("Compressed chunks:\n");
    for (size_t i = 0; i < chunks.count; i++) {
        Chunk original = chunks.chunks[i];
        Chunk compressed = original;
        compress_chunk(&compressed);
        printf("Chunk %zu: ", i + 1);
        for (size_t j = 0; j < compressed.size; j++) {
            if (compressed.data[j] >= 32 && compressed.data[j] <= 126) {
                printf("%c", compressed.data[j]);
            } else {
                printf("[0x%02X]", compressed.data[j]);
            }
        }
        printf(" (compressed from %zu to %zu bytes)\n", original.size, compressed.size);
    }
    free_chunk_array(&chunks);
    return 0;
}