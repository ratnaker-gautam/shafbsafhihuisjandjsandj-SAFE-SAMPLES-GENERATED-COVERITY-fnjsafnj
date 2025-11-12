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

size_t simple_decompress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    for (i = 0; i < input_len; i += 2) {
        if (i + 1 >= input_len) {
            return 0;
        }
        
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

void print_buffer_hex(const uint8_t *data, size_t len) {
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
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    size_t pos = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE - 1) {
        input_buf.data[pos++] = (uint8_t)c;
    }
    input_buf.data[pos] = '\0';
    input_buf.length = pos;
    
    if (input_buf.length == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    size_t compressed_len = simple_compress(input_buf.data, input_buf.length, 
                                          compressed_data, MAX_OUTPUT_SIZE);
    
    if (compressed_len == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_len);
    print_buffer_hex(compressed_data, compressed_len);
    
    size_t decompressed_len = simple_decompress(compressed_data, compressed_len,
                                               decompressed_data, MAX_INPUT_SIZE);
    
    if (decompressed_len == 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_len);
    print_buffer_hex(decompressed_data, decompressed_len);
    
    if (decompressed_len == input_buf.length && 
        memcmp(input_buf.data, decompressed_data, input_buf.length) == 0) {
        printf("\nCompression/Decompression successful!\n");
    } else {
        printf("\nCompression/Decompression failed - data mismatch!\n");
        return 1;
    }
    
    return 0;
}