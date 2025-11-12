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
    
    for (i = 0; i < input_len; i += 2) {
        if (i + 1 >= input_len) {
            break;
        }
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        size_t j;
        for (j = 0; j < count; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) {
                break;
            }
            output[out_idx++] = value;
        }
        
        if (out_idx >= MAX_OUTPUT_SIZE) {
            break;
        }
    }
    
    *output_len = out_idx;
}

int compare_buffers(const uint8_t* buf1, size_t len1, const uint8_t* buf2, size_t len2) {
    if (len1 != len2) {
        return 0;
    }
    
    size_t i;
    for (i = 0; i < len1; i++) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Buffer input, compressed, decompressed;
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    memcpy(input.data, temp_input, input_len);
    input.length = input_len;
    
    simple_compress(input.data, input.length, compressed.data, &compressed.length);
    
    printf("Original size: %zu bytes\n", input.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    if (compressed.length > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input.length - compressed.length) / input.length * 100.0f);
    }
    
    simple_decompress(compressed.data, compressed.length, decompressed.data, &decompressed.length);
    
    if (compare_buffers(input.data, input.length, decompressed.data, decompressed.length)) {
        printf("Decompression successful - data matches original\n");
    } else {
        printf("Decompression failed - data corrupted\n");
        return 1;
    }
    
    printf("Compressed data (hex): ");
    size_t k;
    for (k = 0; k < compressed.length; k++) {
        printf("%02X ", compressed.data[k]);
    }
    printf("\n");
    
    return 0;
}