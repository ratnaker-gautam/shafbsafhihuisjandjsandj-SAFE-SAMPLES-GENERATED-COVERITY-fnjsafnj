//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
    if (data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > MAX_OUTPUT_SIZE) {
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
    buf->size += data_size;
    return 1;
}

int compress_data(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run_length[2] = {0xFF, (uint8_t)count};
            if (!buffer_append(output, run_length, 2)) {
                return 0;
            }
            if (!buffer_append(output, &current, 1)) {
                return 0;
            }
            i += count;
        } else {
            size_t literal_start = i;
            size_t literal_count = 0;
            
            while (i < input_size && literal_count < 255) {
                if (i + 2 < input_size && input[i] == input[i + 1] && input[i] == input[i + 2]) {
                    break;
                }
                literal_count++;
                i++;
            }
            
            if (literal_count > 0) {
                uint8_t literal_header = (uint8_t)(literal_count - 1);
                if (!buffer_append(output, &literal_header, 1)) {
                    return 0;
                }
                if (!buffer_append(output, input + literal_start, literal_count)) {
                    return 0;
                }
            }
        }
    }
    
    return 1;
}

int read_input_data(uint8_t **data, size_t *size) {
    Buffer input_buffer;
    if (!buffer_init(&input_buffer, 4096)) {
        return 0;
    }
    
    uint8_t chunk[1024];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, sizeof(chunk), stdin)) > 0) {
        if (bytes_read > MAX_INPUT_SIZE - input_buffer.size) {
            buffer_free(&input_buffer);
            return 0;
        }
        if (!buffer_append(&input_buffer, chunk, bytes_read)) {
            buffer_free(&input_buffer);
            return 0;
        }
        if (input_buffer.size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    
    if (ferror(stdin)) {
        buffer_free(&input_buffer);
        return 0;
    }
    
    *data = input_buffer.data;
    *size = input_buffer.size;
    return 1;
}

int main(void) {
    uint8_t *input_data = NULL;
    size_t input_size = 0;
    
    if (!read_input_data(&input_data, &input_size)) {
        fprintf(stderr, "Error reading input data\n");
        return 1;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input data provided\n");
        free(input_data);
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_size)) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input_data);
        return 1;
    }
    
    if (!compress_data(input_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        free(input_data);
        buffer_free(&compressed);
        return 1;
    }
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    if (written != compressed.size) {
        fprintf(stderr, "Error writing output\n");
        free(input_data);
        buffer