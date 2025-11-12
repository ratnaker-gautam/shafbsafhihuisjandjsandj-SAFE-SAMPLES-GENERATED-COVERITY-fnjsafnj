//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer* buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(Buffer* buf, size_t needed) {
    if (needed > SIZE_MAX / 2) return 0;
    size_t new_capacity = buf->capacity;
    if (new_capacity == 0) new_capacity = 64;
    while (new_capacity < needed) {
        if (new_capacity > SIZE_MAX / 2) return 0;
        new_capacity *= 2;
    }
    uint8_t* new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t len) {
    if (len > SIZE_MAX - buf->size) return 0;
    if (!buffer_reserve(buf, buf->size + len)) return 0;
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void buffer_free(Buffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_chunk(const uint8_t* input, size_t input_len, Buffer* output) {
    if (input_len == 0) return 1;
    size_t run_length = 1;
    for (size_t i = 1; i < input_len; i++) {
        if (input[i] == input[i-1]) {
            run_length++;
            if (run_length > 255) {
                if (!buffer_append(output, &input[i-1], 1)) return 0;
                if (!buffer_append(output, (uint8_t*)&run_length, 1)) return 0;
                run_length = 1;
            }
        } else {
            if (!buffer_append(output, &input[i-1], 1)) return 0;
            if (!buffer_append(output, (uint8_t*)&run_length, 1)) return 0;
            run_length = 1;
        }
    }
    if (!buffer_append(output, &input[input_len-1], 1)) return 0;
    if (!buffer_append(output, (uint8_t*)&run_length, 1)) return 0;
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    buffer_init(&input);
    buffer_init(&output);
    
    uint8_t chunk[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        if (total_read > SIZE_MAX - bytes_read) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
        total_read += bytes_read;
        
        if (total_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input exceeds maximum size\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
        
        if (!buffer_append(&input, chunk, bytes_read)) {
            fprintf(stderr, "Memory allocation failed\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input.size) {
        size_t chunk_size = input.size - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        if (!compress_chunk(input.data + processed, chunk_size, &output)) {
            fprintf(stderr, "Compression failed\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
        processed += chunk_size;
    }
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error flushing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}