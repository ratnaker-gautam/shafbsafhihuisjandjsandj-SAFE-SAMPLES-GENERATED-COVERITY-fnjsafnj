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

int init_chunk_list(ChunkList *list) {
    if (list == NULL) return 0;
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunk_list(ChunkList *list) {
    if (list == NULL) return;
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_capacity);
        if (new_chunks == NULL) return 0;
        list->chunks = new_chunks;
        list->capacity = new_capacity;
    }
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

void process_chunk(Chunk *chunk) {
    if (chunk == NULL || chunk->size == 0) return;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (chunk->data[i] > 32 && chunk->data[i] < 127) {
            chunk->data[i] = ((chunk->data[i] - 33) * 2) % 94 + 33;
        }
    }
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    ChunkList list;
    if (!init_chunk_list(&list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        if (!add_chunk(&list, (uint8_t*)input + processed, chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_list(&list);
            return 1;
        }
        processed += chunk_size;
    }
    
    for (size_t i = 0; i < list.count; i++) {
        process_chunk(&list.chunks[i]);
    }
    
    printf("Processed data: ");
    for (size_t i = 0; i < list.count; i++) {
        for (size_t j = 0; j < list.chunks[i].size; j++) {
            printf("%c", list.chunks[i].data[j]);
        }
    }
    printf("\n");
    
    printf("Statistics: %zu chunks, %zu bytes input, %zu bytes output\n", 
           list.count, input_len, input_len);
    
    free_chunk_list(&list);
    return 0;
}