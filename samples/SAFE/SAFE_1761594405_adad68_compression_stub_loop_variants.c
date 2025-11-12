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
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_len + 3 > output_max) {
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
    if (!validate_input(input, input_len) || output == NULL || output_max == 0 || input_len % 2 != 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    for (i = 0; i < input_len; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
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

void print_buffer_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    size_t i = 0;
    while (i < len) {
        printf("%02X ", data[i]);
        i++;
        if (i % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_len = sizeof(test_data);
    
    if (test_len > MAX_INPUT_SIZE) {
        return 1;
    }
    
    memcpy(input_buf.data, test_data, test_len);
    input_buf.length = test_len;
    
    printf("Original data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    compressed_buf.length = compressed_size;
    printf("Compressed data (%zu bytes):\n", compressed_buf.length);
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    decompressed_buf.length = decompressed_size;
    printf("Decompressed data (%zu bytes):\n", decompressed_buf.length);
    print_buffer_hex(decompressed_buf.data, decompressed_buf.length);
    
    if (decompressed_buf.length == input_buf.length && 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.length) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
        return 1;
    }
    
    return 0;
}