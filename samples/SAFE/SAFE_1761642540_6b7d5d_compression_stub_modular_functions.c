//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

CompressedData compress_rle(const char* input, size_t len) {
    CompressedData result = {0};
    if (len == 0) return result;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < len && out_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_index + 2 > MAX_OUTPUT_SIZE) break;
        
        result.data[out_index++] = count;
        result.data[out_index++] = current;
        i += count;
    }
    
    result.size = out_index;
    return result;
}

CompressedData compress_data(const char* input, size_t len) {
    CompressedData result = {0};
    if (!validate_input(input, len)) return result;
    
    if (len < 16) {
        if (len > 0 && len <= MAX_OUTPUT_SIZE) {
            memcpy(result.data, input, len);
            result.size = len;
        }
        return result;
    }
    
    return compress_rle(input, len);
}

void print_compressed(const CompressedData* compressed) {
    if (compressed == NULL || compressed->size == 0) return;
    
    printf("Compressed data (%zu bytes):\n", compressed->size);
    for (size_t i = 0; i < compressed->size; i++) {
        if (i > 0 && i % 16 == 0) printf("\n");
        printf("%02X ", compressed->data[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1] = {0};
    size_t input_len = 0;
    
    printf("Enter text to compress (max %d characters):\n", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    CompressedData compressed = compress_data(input, input_len);
    
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    print_compressed(&compressed);
    
    double ratio = (double)compressed.size / (double)input_len * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}