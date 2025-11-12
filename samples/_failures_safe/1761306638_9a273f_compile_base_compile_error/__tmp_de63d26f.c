//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size > SIZE_MAX - len) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > MAX_OUTPUT_SIZE) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run[3] = {0xFF, current, (uint8_t)count};
            if (!buffer_append(output, run, 3)) return 0;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                uint8_t literal[2] = {0x00, *(input + i + j)};
                if (!buffer_append(output, literal, 2)) return 0;
            }
            i += count;
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (i + 1 >= input_len) return 0;
        
        uint8_t marker = *(input + i);
        uint8_t value = *(input + i + 1);
        
        if (marker == 0xFF) {
            if (i + 2 >= input_len) return 0;
            uint8_t count = *(input + i + 2);
            if (count == 0) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else if (marker == 0x00) {
            if (!buffer_append(output, &value, 1)) return 0;
            i += 2;
        } else {
            return 0;
        }
    }
    return 1;
}

int read_input(Buffer *buf) {
    if (buf == NULL) return 0;
    
    uint8_t temp[256];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(temp, 1, sizeof(temp), stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(buf, temp, bytes_read)) return 0;
        total_read += bytes_read;
        
        if (total_read >= MAX_INPUT_SIZE || bytes_read < sizeof(temp)) break;
    }
    
    return total_read > 0;
}

void write_output(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    fwrite(buf->data, 1, buf->size, stdout);
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    if (!buffer_init(&input, 1024)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&compressed, 2048)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input);
        return 1;
    }
    
    if (!buffer_init(&decompressed, 1024)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input