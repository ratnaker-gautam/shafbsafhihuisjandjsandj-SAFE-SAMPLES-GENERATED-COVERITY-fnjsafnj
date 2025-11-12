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

void initialize_buffer(Buffer *buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const uint8_t *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_idx + 2 >= output_max) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        for (; j < input_len; j++) {
            if (input[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        output[output_idx++] = current;
        output[output_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0 || input_len % 2 != 0) {
        return 0;
    }

    size_t output_idx = 0;
    
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t character = input[i];
        uint8_t count = input[i + 1];
        
        if (output_idx + count > output_max) {
            return 0;
        }
        
        size_t k = 0;
        while (k < count) {
            output[output_idx++] = character;
            k++;
        }
    }
    
    return output_idx;
}

void print_buffer_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
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
    
    uint8_t test_data[] = "AAAABBBCCDAA";
    size_t test_len = strlen((char*)test_data);
    
    if (test_len > MAX_INPUT_SIZE) {
        return 1;
    }
    
    memcpy(input_buf.data, test_data, test_len);
    input_buf.length = test_len;
    
    printf("Original data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    printf("As string: %s\n\n", input_buf.data);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    compressed_buf.length = compressed_size;
    printf("Compressed data (%zu bytes):\n", compressed_buf.length);
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    float ratio = (float)compressed_size / input_buf.length;
    printf("Compression ratio: %.2f\n\n", ratio);
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    decompressed_buf.length = decompressed_size;
    printf("Decompressed data (%zu bytes):\n", decompressed_buf.length);
    print_buffer_hex(decompressed_buf.data, decompressed_buf.length);
    printf("As string: %s\n", decompressed_buf.data);
    
    if (decompressed_size == input_buf.length && 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}