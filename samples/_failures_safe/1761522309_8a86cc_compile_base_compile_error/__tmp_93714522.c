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
    uint8_t *stack_data;
    size_t stack_size;
    Buffer *heap_buffer;
} Compressor;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

int compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0) return 0;
    if (input_size > CHUNK_SIZE) return 0;
    size_t compressed = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t count = 1;
        while (i + count < input_size && input[i] == input[i + count] && count < 255) {
            count++;
        }
        if (compressed + 2 > CHUNK_SIZE * 2) return 0;
        output[compressed++] = count;
        output[compressed++] = input[i];
        i += count;
    }
    *output_size = compressed;
    return 1;
}

int compressor_init(Compressor *comp) {
    if (comp == NULL) return 0;
    comp->stack_data = NULL;
    comp->stack_size = 0;
    comp->heap_buffer = malloc(sizeof(Buffer));
    if (comp->heap_buffer == NULL) return 0;
    if (!buffer_init(comp->heap_buffer, 256)) {
        free(comp->heap_buffer);
        comp->heap_buffer = NULL;
        return 0;
    }
    return 1;
}

void compressor_free(Compressor *comp) {
    if (comp != NULL) {
        buffer_free(comp->heap_buffer);
        free(comp->heap_buffer);
        comp->heap_buffer = NULL;
    }
}

int compressor_process(Compressor *comp, const uint8_t *input, size_t input_size) {
    if (comp == NULL || input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    uint8_t stack_chunk[CHUNK_SIZE * 2];
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t compressed_size;
        if (!compress_chunk(input + i, chunk_size, stack_chunk, &compressed_size)) return 0;
        if (!buffer_append(comp->heap_buffer, stack_chunk, compressed_size)) return 0;
    }
    return 1;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    Compressor comp;
    if (!compressor_init(&comp)) {
        printf("Compressor initialization failed\n");
        return 1;
    }
    if (!compressor_process(&