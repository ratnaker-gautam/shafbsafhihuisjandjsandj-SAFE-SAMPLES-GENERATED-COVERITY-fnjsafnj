//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
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
    if (buf == NULL || src == NULL) return 0;
    if (len == 0) return 1;
    
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity * sizeof(uint8_t));
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
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
        
        if (count > 3 || current == 0xFF) {
            uint8_t run_length = (count > UINT8_MAX) ? UINT8_MAX : count;
            uint8_t marker[] = {0xFF, run_length, current};
            if (!buffer_append(output, marker, 3)) return 0;
        } else {
            if (!buffer_append(output, &current, 1)) return 0;
            if (count > 1) {
                uint8_t repeat = count - 1;
                if (!buffer_append(output, &repeat, 1)) return 0;
            }
        }
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        
        if (current == 0xFF && i + 2 < input_len) {
            uint8_t count = *(input + i + 1);
            uint8_t value = *(input + i + 2);
            
            if (count == 0) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            
            i += 3;
        } else {
            if (!buffer_append(output, &current, 1)) return 0;
            i++;
            
            if (i < input_len && *(input + i) > 0 && *(input + i) < 0xFF) {
                uint8_t repeat = *(input + i);
                for (uint8_t j = 0; j < repeat; j++) {
                    if (!buffer_append(output, &current, 1)) return 0;
                }
                i++;
            }
        }
    }
    
    return 1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&compressed_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    if (!buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return 1;
    }
    
    printf("Enter data to compress (max %