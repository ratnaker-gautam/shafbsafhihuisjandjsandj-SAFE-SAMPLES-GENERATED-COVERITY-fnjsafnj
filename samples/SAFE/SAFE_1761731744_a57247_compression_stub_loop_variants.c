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
    size_t pair_count = input_len / 2;
    size_t pair_index = 0;
    
    for (pair_index = 0; pair_index < pair_count; pair_index++) {
        uint8_t character = input[pair_index * 2];
        uint8_t count = input[pair_index * 2 + 1];
        
        if (output_idx + count > output_size) {
            return 0;
        }
        
        size_t k = 0;
        do {
            output[output_idx++] = character;
            k++;
        } while (k < count);
    }
    
    return output_idx;
}

void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    size_t idx = 0;
    while (idx < len) {
        printf("%02X ", data[idx]);
        idx++;
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
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    compressed_buf.length = compressed_size;
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed_buf.length = decompressed_size;
    
    printf("Original: %s\n", input_buf.data);
    printf("Compressed (hex): ");
    print_hex(compressed_buf.data, compressed_buf.length);
    printf("Decompressed: %s\n", decompressed_buf.data);
    
    if (memcmp(input_buf.data, decompressed_buf.data, input_buf.length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed_buf.length / input_buf.length * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}