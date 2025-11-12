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

void simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    *output_len = out_idx;
}

void simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input_len) {
        if (in_idx + 1 >= input_len) {
            break;
        }
        
        uint8_t character = input[in_idx++];
        uint8_t count = input[in_idx++];
        
        size_t k = 0;
        do {
            if (out_idx >= MAX_OUTPUT_SIZE) {
                break;
            }
            output[out_idx++] = character;
            k++;
        } while (k < count);
    }
    
    *output_len = out_idx;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    for (size_t idx = 0; idx < len; idx++) {
        printf("%02X ", data[idx]);
    }
    printf("\n");
}

int main(void) {
    Buffer input_buf;
    Buffer compressed_buf;
    Buffer decompressed_buf;
    
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen(temp_input);
    if (input_length > 0 && temp_input[input_length - 1] == '\n') {
        temp_input[input_length - 1] = '\0';
        input_length--;
    }
    
    if (!validate_input((uint8_t*)temp_input, input_length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_length);
    input_buf.length = input_length;
    
    printf("Original data (%zu bytes): ", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    size_t compressed_len = 0;
    simple_compress(input_buf.data, input_buf.length, compressed_buf.data, &compressed_len);
    compressed_buf.length = compressed_len;
    
    printf("Compressed data (%zu bytes): ", compressed_buf.length);
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    size_t decompressed_len = 0;
    simple_decompress(compressed_buf.data, compressed_buf.length, decompressed_buf.data, &decompressed_len);
    decompressed_buf.length = decompressed_len;
    
    printf("Decompressed data (%zu bytes): ", decompressed_buf.length);
    print_buffer_hex(decompressed_buf.data, decompressed_buf.length);
    
    if (input_buf.length == decompressed_buf.length && 
        memcmp(input_buf.data, decompressed_buf.data, input_buf.length) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
        return 1;
    }
    
    return 0;
}