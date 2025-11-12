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

void simple_compress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        for (; j < input->length; j++) {
            if (input->data[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        i = j;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        if (i + 1 >= input->length) {
            break;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
        
        i += 2;
    }
    
    output->length = out_idx;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t idx = 0; idx < buf->length; idx++) {
        printf("%02X ", buf->data[idx]);
    }
    printf("\n");
}

int main() {
    Buffer input, compressed, decompressed;
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input((uint8_t*)user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input.data, user_input, input_len);
    input.length = input_len;
    
    printf("Original data (%zu bytes): ", input.length);
    print_buffer_hex(&input);
    
    simple_compress(&input, &compressed);
    printf("Compressed data (%zu bytes): ", compressed.length);
    print_buffer_hex(&compressed);
    
    simple_decompress(&compressed, &decompressed);
    printf("Decompressed data (%zu bytes): ", decompressed.length);
    print_buffer_hex(&decompressed);
    
    if (input.length == decompressed.length && 
        memcmp(input.data, decompressed.data, input.length) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
        return 1;
    }
    
    return 0;
}