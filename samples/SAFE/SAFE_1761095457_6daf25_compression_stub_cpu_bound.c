//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

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
    if (data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity *= 2;
            if (new_capacity > MAX_INPUT_SIZE) {
                return 0;
            }
        }
        uint8_t* new_data = realloc(buf->data, new_capacity);
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

void compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            size_t literal_start = in_idx;
            while (in_idx < input_size) {
                if (in_idx + 2 < input_size && input[in_idx] == input[in_idx + 1] && input[in_idx] == input[in_idx + 2]) {
                    break;
                }
                in_idx++;
                if (in_idx - literal_start >= 255) {
                    break;
                }
            }
            size_t literal_count = in_idx - literal_start;
            output[out_idx++] = (uint8_t)literal_count;
            for (size_t i = 0; i < literal_count; i++) {
                output[out_idx++] = input[literal_start + i];
            }
        }
    }
    *output_size = out_idx;
}

int main(void) {
    Buffer input_buf;
    if (!buffer_init(&input_buf, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input_buf, chunk, bytes_read)) {
            fprintf(stderr, "Failed to append data to buffer\n");
            buffer_free(&input_buf);
            return 1;
        }
    }
    
    if (!feof(stdin)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Input too large\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    uint8_t* output_data = malloc(max_output_size);
    if (output_data == NULL) {
        fprintf(stderr, "Failed to allocate output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    size_t output_size;
    compress_chunk(input_buf.data, input_buf.size, output_data, &output_size);
    
    if (fwrite(output_data, 1, output_size, stdout) != output_size) {
        fprintf(stderr, "Failed to write output\n");
        free(output_data);
        buffer_free(&input_buf);
        return 1;
    }
    
    free(output_data);
    buffer_free(&input_buf);
    return 0;
}