//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

typedef struct {
    uint8_t stack_data[CHUNK_SIZE];
    size_t stack_used;
    Buffer *heap_buffer;
} Compressor;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (data_size == 0) {
        return 1;
    }
    if (buf->size > SIZE_MAX - data_size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity < buf->capacity) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void compressor_init(Compressor *comp) {
    comp->stack_used = 0;
    comp->heap_buffer = malloc(sizeof(Buffer));
    if (comp->heap_buffer != NULL) {
        if (!buffer_init(comp->heap_buffer, CHUNK_SIZE)) {
            free(comp->heap_buffer);
            comp->heap_buffer = NULL;
        }
    }
}

void compressor_free(Compressor *comp) {
    if (comp->heap_buffer != NULL) {
        buffer_free(comp->heap_buffer);
        free(comp->heap_buffer);
    }
}

int compressor_process_chunk(Compressor *comp, const uint8_t *chunk, size_t chunk_size) {
    if (chunk_size == 0) {
        return 1;
    }
    if (chunk_size <= CHUNK_SIZE - comp->stack_used) {
        memcpy(comp->stack_data + comp->stack_used, chunk, chunk_size);
        comp->stack_used += chunk_size;
        return 1;
    }
    if (comp->heap_buffer == NULL) {
        return 0;
    }
    if (comp->stack_used > 0) {
        if (!buffer_append(comp->heap_buffer, comp->stack_data, comp->stack_used)) {
            return 0;
        }
        comp->stack_used = 0;
    }
    return buffer_append(comp->heap_buffer, chunk, chunk_size);
}

int compressor_finalize(Compressor *comp, Buffer *output) {
    if (comp->heap_buffer == NULL) {
        return 0;
    }
    if (comp->stack_used > 0) {
        if (!buffer_append(comp->heap_buffer, comp->stack_data, comp->stack_used)) {
            return 0;
        }
        comp->stack_used = 0;
    }
    if (!buffer_init(output, comp->heap_buffer->size)) {
        return 0;
    }
    for (size_t i = 0; i < comp->heap_buffer->size; i++) {
        output->data[i] = comp->heap_buffer->data[i] ^ 0x55;
    }
    output->size = comp->heap_buffer->size;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
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
    Compressor comp;
    compressor_init(&comp);
    if (comp.heap_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        if (!compressor_process_chunk(&comp, (uint8_t*)input + processed, chunk_size)) {
            fprintf(st