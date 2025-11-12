//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (output_size < input_size * 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 2 > output_size) {
                return 0;
            }
            output[output_idx++] = 0xFF;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            if (output_idx + count > output_size) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t read_input(Buffer *buf) {
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE / 2);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        return 0;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len == 0) {
        return 0;
    }
    
    size_t data_size = hex_len / 2;
    if (data_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < hex_len; i++) {
        if (!isxdigit(hex_input[i])) {
            return 0;
        }
    }
    
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        buf->data[i] = (uint8_t)strtoul(hex_byte, NULL, 16);
    }
    
    buf->size = data_size;
    return data_size;
}

void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input_buf = {0};
    Buffer output_buf = {0};
    
    if (read_input(&input_buf) == 0) {
        fprintf(stderr, "Error: Invalid input format or size\n");
        return 1;
    }
    
    if (!validate_input(input_buf.data, input_buf.size)) {
        fprintf(stderr, "Error: Input validation failed\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_buf.size);
    print_hex(input_buf.data, input_buf.size);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, 
                                           output_buf.data, sizeof(output_buf.data));
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    print_hex(output_buf.data, compressed_size);
    
    double ratio = (double)compressed_size / input_buf.size * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}