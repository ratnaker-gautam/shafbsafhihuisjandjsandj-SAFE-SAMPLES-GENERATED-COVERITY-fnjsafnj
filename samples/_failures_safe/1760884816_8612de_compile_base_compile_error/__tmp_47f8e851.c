//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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
} Buffer;

Buffer create_buffer(size_t size) {
    Buffer buf;
    buf.data = NULL;
    buf.size = 0;
    
    if (size > 0 && size <= MAX_OUTPUT_SIZE) {
        buf.data = malloc(size);
        if (buf.data != NULL) {
            buf.size = size;
        }
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
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_data(const uint8_t *input, size_t input_size) {
    Buffer output = create_buffer(0);
    
    if (!validate_input(input, input_size)) {
        return output;
    }
    
    size_t max_output_size = input_size * 2;
    if (max_output_size > MAX_OUTPUT_SIZE) {
        max_output_size = MAX_OUTPUT_SIZE;
    }
    
    output = create_buffer(max_output_size);
    if (output.data == NULL) {
        return output;
    }
    
    size_t output_pos = 0;
    
    for (size_t i = 0; i < input_size && output_pos + 2 < output.size; i++) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        
        if (output_pos + 2 <= output.size) {
            output.data[output_pos++] = count;
            output.data[output_pos++] = current;
        } else {
            break;
        }
    }
    
    output.size = output_pos;
    return output;
}

Buffer decompress_data(const uint8_t *input, size_t input_size) {
    Buffer output = create_buffer(0);
    
    if (!validate_input(input, input_size)) {
        return output;
    }
    
    if (input_size % 2 != 0) {
        return output;
    }
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (i + 1 < input_size) {
            uint8_t count = input[i];
            if (count == 0) {
                return create_buffer(0);
            }
            if (estimated_size > SIZE_MAX - count) {
                return create_buffer(0);
            }
            estimated_size += count;
        }
    }
    
    if (estimated_size == 0 || estimated_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    output = create_buffer(estimated_size);
    if (output.data == NULL) {
        return output;
    }
    
    size_t output_pos = 0;
    
    for (size_t i = 0; i < input_size && output_pos < output.size; i += 2) {
        if (i + 1 >= input_size) break;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_pos > output.size - count) {
            break;
        }
        
        for (uint8_t j = 0; j < count && output_pos < output.size; j++) {
            output.data[output_pos++] = value;
        }
    }
    
    output.size = output_pos;
    return output;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buf->size > 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", test_size);
    print_buffer_hex(&(Buffer){test_data, test_size});
    
    Buffer compressed = compress_data(test_data, test_size