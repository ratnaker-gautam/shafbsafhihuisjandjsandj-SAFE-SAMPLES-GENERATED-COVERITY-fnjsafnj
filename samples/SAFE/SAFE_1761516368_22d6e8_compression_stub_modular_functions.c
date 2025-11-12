//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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
} Buffer;

Buffer create_buffer(size_t size) {
    Buffer buf;
    buf.data = NULL;
    buf.size = 0;
    
    if (size == 0 || size > MAX_OUTPUT_SIZE) {
        return buf;
    }
    
    buf.data = malloc(size);
    if (buf.data != NULL) {
        buf.size = size;
    }
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

Buffer compress_data(const uint8_t *input, size_t input_size) {
    Buffer output = create_buffer(0);
    
    if (!validate_input(input, input_size)) {
        return output;
    }
    
    size_t max_output_size = input_size * 2;
    if (max_output_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    output = create_buffer(max_output_size);
    if (output.data == NULL) {
        return output;
    }
    
    size_t output_idx = 0;
    
    for (size_t i = 0; i < input_size; i++) {
        if (output_idx + 2 >= max_output_size) {
            destroy_buffer(&output);
            return create_buffer(0);
        }
        
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < UINT8_MAX) {
            count++;
            i++;
        }
        
        output.data[output_idx++] = current;
        output.data[output_idx++] = count;
    }
    
    output.size = output_idx;
    return output;
}

Buffer decompress_data(const uint8_t *input, size_t input_size) {
    Buffer output = create_buffer(0);
    
    if (!validate_input(input, input_size) || input_size % 2 != 0) {
        return output;
    }
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (estimated_size > SIZE_MAX - input[i + 1]) {
            return output;
        }
        estimated_size += input[i + 1];
    }
    
    if (estimated_size == 0 || estimated_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    output = create_buffer(estimated_size);
    if (output.data == NULL) {
        return output;
    }
    
    size_t output_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output_idx + count > estimated_size) {
            destroy_buffer(&output);
            return create_buffer(0);
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output.data[output_idx++] = value;
        }
    }
    
    output.size = output_idx;
    return output;
}

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    printf("Original data: ");
    for (size_t i = 0; i < test_size; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(test_data, test_size);
    if (compressed.data != NULL) {
        printf("Compressed: ");
        print_buffer(&compressed);
        
        Buffer decompressed = decompress_data(compressed.data, compressed.size);
        if (decompressed.data != NULL) {
            printf("Decompressed: ");
            print_buffer(&decompressed);
            
            if (decompressed.size == test_size && 
                memcmp(decompressed.data, test_data, test_size) == 0) {
                printf("Compression test passed\n");
            } else {
                printf("Compression test failed\n");
            }
            destroy_buffer(&decompressed);
        } else {
            printf("Decompression failed\n");
        }
        destroy_buffer(&compressed);
    } else {
        printf("Compression failed\n");
    }
    
    return 0;
}