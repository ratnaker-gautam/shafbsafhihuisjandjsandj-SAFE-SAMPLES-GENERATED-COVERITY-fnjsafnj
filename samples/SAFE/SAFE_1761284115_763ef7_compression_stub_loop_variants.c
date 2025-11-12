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
    if (!validate_input(input, input_len) || output == NULL || output_len == NULL) {
        *output_len = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    *output_len = out_idx;
}

void simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    if (!validate_input(input, input_len) || output == NULL || output_len == NULL) {
        *output_len = 0;
        return;
    }

    size_t out_idx = 0;
    size_t i = 0;
    
    for (; i < input_len - 1; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    *output_len = out_idx;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    size_t i = 0;
    while (i < len) {
        printf("%02X ", data[i]);
        i++;
    }
    printf("\n");
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    simple_compress(input_buf.data, input_buf.length, compressed_buf.data, &compressed_buf.length);
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    
    printf("Compressed data (hex): ");
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    simple_decompress(compressed_buf.data, compressed_buf.length, decompressed_buf.data, &decompressed_buf.length);
    
    printf("Decompressed size: %zu bytes\n", decompressed_buf.length);
    printf("Decompressed text: ");
    
    size_t idx = 0;
    while (idx < decompressed_buf.length) {
        putchar(decompressed_buf.data[idx]);
        idx++;
    }
    printf("\n");
    
    if (input_buf.length == decompressed_buf.length && 
        memcmp(input_buf.data, decompressed_buf.data, input_buf.length) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
    }
    
    return 0;
}