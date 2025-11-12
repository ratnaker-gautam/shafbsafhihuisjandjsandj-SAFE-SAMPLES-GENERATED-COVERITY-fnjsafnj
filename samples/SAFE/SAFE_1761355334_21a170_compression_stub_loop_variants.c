//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_len + 2 > output_max) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output[output_len++] = current;
        output[output_len++] = (uint8_t)count;
        
        i = j;
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input_len == 0 || input_len % 2 != 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    
    for (size_t idx = 0; idx < input_len; idx += 2) {
        uint8_t value = input[idx];
        uint8_t count = input[idx + 1];
        
        if (output_len + count > output_max) {
            return 0;
        }
        
        size_t k = 0;
        do {
            output[output_len++] = value;
            k++;
        } while (k < count);
    }
    
    return output_len;
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
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    compressed_buf.length = compressed_size;
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size != input_buf.length) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input_buf.data, decompressed_buf.data, input_buf.length) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.length / input_buf.length) * 100.0);
    
    printf("Decompression verified: OK\n");
    
    return 0;
}