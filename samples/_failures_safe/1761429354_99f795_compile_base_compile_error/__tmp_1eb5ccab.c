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
    if (initial_capacity > SIZE_MAX / 2) return 0;
    
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    size_t i = 0;
    while (i < data_size) {
        *(dest + i) = *(src + i);
        i++;
    }
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               *(input + i + count) == current) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run_length = (count > UINT8_MAX) ? UINT8_MAX : count;
            uint8_t run_header[2] = {0xFF, run_length};
            if (!buffer_append(output, run_header, 2)) return 0;
            if (!buffer_append(output, &current, 1)) return 0;
            i += count;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_size && literal_count < UINT8_MAX && 
                   (literal_count + 1 >= input_size || 
                    *(input + i + literal_count) != *(input + i + literal_count + 1) ||
                    (i + literal_count + 2 < input_size && 
                     *(input + i + literal_count) == *(input + i + literal_count + 1) &&
                     *(input + i + literal_count) == *(input + i + literal_count + 2)))) {
                literal_count++;
                if (literal_count == UINT8_MAX) break;
            }
            
            uint8_t literal_header = literal_count - 1;
            if (!buffer_append(output, &literal_header, 1)) return 0;
            if (!buffer_append(output, input + i, literal_count)) return 0;
            i += literal_count;
        }
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t header = *(input + i);
        i++;
        
        if (header == 0xFF) {
            if (i >= input_size) return 0;
            uint8_t run_length = *(input + i);
            i++;
            if (i >= input_size) return 0;
            uint8_t value = *(input + i);
            i++;
            
            if (run_length == 0) return 0;
            
            size_t j = 0;
            while (j < run_length) {
                if (!buffer_append(output, &value, 1)) return 0;
                j++;
            }
        } else {
            size_t literal_count = header + 1;
            if (i + literal_count > input_size) return 0;
            
            if (!buffer_append(output, input + i, literal_count)) return 0;
            i += literal_count;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter input data (up to %d bytes):