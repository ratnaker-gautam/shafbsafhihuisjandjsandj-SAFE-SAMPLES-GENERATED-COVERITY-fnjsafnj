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

void generate_test_data(Buffer* buf) {
    const char* test_pattern = "AAAABBBCCDEEEEFFGGGHHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";
    size_t pattern_len = strlen(test_pattern);
    
    if (pattern_len > MAX_INPUT_SIZE) {
        return;
    }
    
    for (size_t idx = 0; idx < pattern_len; idx++) {
        buf->data[idx] = (uint8_t)test_pattern[idx];
    }
    buf->length = pattern_len;
}

int main(void) {
    Buffer input_buf;
    Buffer compressed_buf;
    Buffer decompressed_buf;
    
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    generate_test_data(&input_buf);
    
    if (input_buf.length == 0) {
        fprintf(stderr, "Error: Failed to generate test data\n");
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    compressed_buf.length = compressed_size;
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Error: Decompression failed\n");
        return 1;
    }
    decompressed_buf.length = decompressed_size;
    
    if (input_buf.length != decompressed_buf.length) {
        fprintf(stderr, "Error: Size mismatch after decompression\n");
        return 1;
    }
    
    int match = 1;
    for (size_t pos = 0; pos < input_buf.length; pos++) {
        if (input_buf.data[pos] != decompressed_buf.data[pos]) {
            match = 0;
            break;
        }
    }
    
    if (!match) {
        fprintf(stderr, "Error: Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression test successful\n");
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.length / input_buf.length) * 100.0);
    
    return 0;
}