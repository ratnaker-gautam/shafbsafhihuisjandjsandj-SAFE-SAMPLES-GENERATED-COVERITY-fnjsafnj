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
    if (initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void compressor_init(Compressor *comp) {
    comp->stack_used = 0;
    comp->heap_buffer = malloc(sizeof(Buffer));
    if (comp->heap_buffer) {
        if (!buffer_init(comp->heap_buffer, CHUNK_SIZE * 2)) {
            free(comp->heap_buffer);
            comp->heap_buffer = NULL;
        }
    }
}

void compressor_free(Compressor *comp) {
    if (comp) {
        if (comp->heap_buffer) {
            buffer_free(comp->heap_buffer);
            free(comp->heap_buffer);
        }
    }
}

int compressor_process_chunk(Compressor *comp, const uint8_t *data, size_t len) {
    if (!comp || !data || len == 0) return 0;
    if (len <= CHUNK_SIZE - comp->stack_used) {
        memcpy(comp->stack_data + comp->stack_used, data, len);
        comp->stack_used += len;
        return 1;
    } else {
        if (comp->stack_used > 0) {
            if (!buffer_append(comp->heap_buffer, comp->stack_data, comp->stack_used)) return 0;
            comp->stack_used = 0;
        }
        if (len > CHUNK_SIZE) {
            return buffer_append(comp->heap_buffer, data, len);
        } else {
            memcpy(comp->stack_data, data, len);
            comp->stack_used = len;
            return 1;
        }
    }
}

void compressor_finalize(Compressor *comp) {
    if (!comp) return;
    if (comp->stack_used > 0 && comp->heap_buffer) {
        buffer_append(comp->heap_buffer, comp->stack_data, comp->stack_used);
        comp->stack_used = 0;
    }
}

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_len == 0 || output_capacity == 0) return 0;
    size_t out_pos = 0;
    for (size_t i = 0; i < input_len; ) {
        if (out_pos >= output_capacity) return 0;
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_pos + 2 > output_capacity) return 0;
        output[out_pos++] = count;
        output[out_pos++] = current;
        i += count;
    }
    return out_pos;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
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
    if (!comp.heap_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    if (!compressor_process_chunk(&comp, (uint8_t*)input, input_len)) {
        fprintf(stderr, "Processing failed\n");
        compressor_free(&comp);
        return 1;
    }
    compressor_finalize(&comp);

    size_t total_size