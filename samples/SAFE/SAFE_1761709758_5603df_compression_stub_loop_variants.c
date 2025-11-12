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

void process_compression() {
    Buffer input_buf, compressed_buf, decompressed_buf;
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    compressed_buf.length = compressed_size;
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size != input_buf.length) {
        printf("Decompression failed\n");
        return;
    }
    
    int match = 1;
    size_t idx = 0;
    while (idx < input_buf.length) {
        if (input_buf.data[idx] != decompressed_buf.data[idx]) {
            match = 0;
            break;
        }
        idx++;
    }
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buf.length / input_buf.length) * 100);
    
    if (match) {
        printf("Verification: PASS - decompressed data matches original\n");
    } else {
        printf("Verification: FAIL - data mismatch\n");
    }
}

int main() {
    process_compression();
    return 0;
}