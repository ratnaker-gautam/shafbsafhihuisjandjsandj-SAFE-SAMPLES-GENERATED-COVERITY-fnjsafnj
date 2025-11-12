//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_size) {
    if (output_size < input_len * 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count > 3 || current == '\\') {
            if (output_idx + 3 >= output_size) {
                return 0;
            }
            output[output_idx++] = '\\';
            output[output_idx++] = count;
            output[output_idx++] = current;
        } else {
            for (size_t k = 0; k < count; k++) {
                if (output_idx >= output_size) {
                    return 0;
                }
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_size) {
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (input[i] == '\\' && i + 2 < input_len) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + count > output_size) {
                return 0;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            i += 3;
        } else {
            if (output_idx >= output_size) {
                return 0;
            }
            output[output_idx++] = input[i];
            i++;
        }
    }
    
    return output_idx;
}

int main(void) {
    Buffer input_buf;
    Buffer compressed_buf;
    Buffer decompressed_buf;
    
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buf.data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_buf.length = strlen((char*)input_buf.data);
    if (input_buf.length > 0 && input_buf.data[input_buf.length - 1] == '\n') {
        input_buf.data[--input_buf.length] = '\0';
    }
    
    if (!validate_input(input_buf.data, input_buf.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    compressed_buf.length = simple_compress(input_buf.data, input_buf.length, 
                                          compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buf.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    decompressed_buf.length = simple_decompress(compressed_buf.data, compressed_buf.length,
                                               decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_buf.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_buf.length != input_buf.length || 
        memcmp(input_buf.data, decompressed_buf.data, input_buf.length) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.length / input_buf.length) * 100);
    
    printf("Decompression verified successfully\n");
    
    return 0;
}