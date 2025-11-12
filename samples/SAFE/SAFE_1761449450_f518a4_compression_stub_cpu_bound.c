//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer* buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
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

void buffer_free(Buffer* buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (data_size > MAX_INPUT_SIZE - buf->size) {
        return 0;
    }
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
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

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size == 0 || output == NULL) {
        return;
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
            if (!buffer_append(output, run_length, 2)) return;
            if (!buffer_append(output, &current, 1)) return;
            i += count;
        } else {
            size_t literal_start = i;
            size_t literal_end = i;
            
            while (literal_end < input_size && 
                   (literal_end - i < 127) && 
                   (literal_end + 1 >= input_size || 
                    input[literal_end] != input[literal_end + 1] || 
                    literal_end + 2 >= input_size || 
                    input[literal_end] != input[literal_end + 2])) {
                literal_end++;
            }
            
            size_t literal_length = literal_end - i;
            if (literal_length > 0) {
                uint8_t length_byte = (uint8_t)literal_length;
                if (!buffer_append(output, &length_byte, 1)) return;
                if (!buffer_append(output, input + i, literal_length)) return;
                i += literal_length;
            }
        }
    }
}

int read_input(Buffer* input) {
    if (!buffer_init(input, CHUNK_SIZE)) {
        return 0;
    }
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            buffer_free(input);
            return 0;
        }
        if (input->size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    
    if (ferror(stdin)) {
        buffer_free(input);
        return 0;
    }
    
    return 1;
}

int main(void) {
    Buffer input_data;
    Buffer compressed_data;
    
    if (!read_input(&input_data)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!buffer_init(&compressed_data, input_data.size)) {
        buffer_free(&input_data);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress(input_data.data, input_data.size, &compressed_data);
    
    if (fwrite(compressed_data.data, 1, compressed_data.size, stdout) != compressed_data.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    buffer_free(&input_data);
    buffer_free(&compressed_data);
    
    return 0;
}