//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
} ChunkList;

int init_chunklist(ChunkList *list) {
    if (list == NULL) return 0;
    list->capacity = 8;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    if (list->chunks == NULL) return 0;
    return 1;
}

void free_chunklist(ChunkList *list) {
    if (list == NULL) return;
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_cap);
        if (new_chunks == NULL) return 0;
        list->chunks = new_chunks;
        list->capacity = new_cap;
    }
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

void process_chunk_stack(Chunk *chunk) {
    if (chunk == NULL) return;
    for (size_t i = 0; i < chunk->size; i++) {
        chunk->data[i] = (chunk->data[i] + 1) % 256;
    }
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    ChunkList list;
    if (!init_chunklist(&list)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        if (!add_chunk(&list, (uint8_t*)input + processed, chunk_size)) {
            printf("Chunk processing failed\n");
            free_chunklist(&list);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Processing %zu chunks:\n", list.count);
    for (size_t i = 0; i < list.count; i++) {
        Chunk local_chunk = list.chunks[i];
        process_chunk_stack(&local_chunk);
        printf("Chunk %zu: ", i + 1);
        for (size_t j = 0; j < local_chunk.size; j++) {
            printf("%02X ", local_chunk.data[j]);
        }
        printf("\n");
    }
    free_chunklist(&list);
    return 0;
}