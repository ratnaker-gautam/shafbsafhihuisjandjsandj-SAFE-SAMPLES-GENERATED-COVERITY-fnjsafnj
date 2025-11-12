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

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_len) || output == NULL || output_size < input_len * 2) {
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
        
        if (output_idx + 2 > output_size) {
            return 0;
        }
        
        output[output_idx++] = current;
        output[output_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_len) || output == NULL || input_len % 2 != 0) {
        return 0;
    }

    size_t output_idx = 0;
    
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output_idx + count > output_size) {
            return 0;
        }
        
        size_t k = 0;
        do {
            output[output_idx++] = value;
            k++;
        } while (k < count);
    }
    
    return output_idx;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buf;
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    initialize_buffer(&input_buf);
    
    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    size_t pos = 0;
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE - 1) {
        if (isprint(c) || c == ' ' || c == '\t') {
            input_buf.data[pos++] = (uint8_t)c;
        }
    }
    input_buf.length = pos;
    
    if (input_buf.length == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(decompressed_data, decompressed_size);
    
    if (decompressed_size == input_buf.length && memcmp(input_buf.data, decompressed_data, input_buf.length) == 0) {
        printf("\nCompression/Decompression successful!\n");
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.length) * 100.0);
    } else {
        printf("\nCompression/Decompression failed - data mismatch!\n");
        return 1;
    }
    
    return 0;
}