//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF
#define MAX_RUN_LENGTH 255

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_rle(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    size_t i = 0;
    
    while (i < input_size && output.size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output.size + 2 >= MAX_OUTPUT_SIZE) break;
            output.data[output.size++] = ESCAPE_CHAR;
            output.data[output.size++] = (uint8_t)run_length;
            output.data[output.size++] = current;
            i += run_length;
        } else {
            for (size_t j = 0; j < run_length && output.size < MAX_OUTPUT_SIZE; j++) {
                if (current == ESCAPE_CHAR) {
                    if (output.size + 2 >= MAX_OUTPUT_SIZE) break;
                    output.data[output.size++] = ESCAPE_CHAR;
                    output.data[output.size++] = 1;
                }
                output.data[output.size++] = current;
            }
            i += run_length;
        }
    }
    
    return output;
}

Buffer decompress_rle(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    size_t i = 0;
    
    while (i < input_size && output.size < MAX_OUTPUT_SIZE) {
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_size) break;
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (run_length == 0 || output.size + run_length > MAX_OUTPUT_SIZE) {
                break;
            }
            
            for (uint8_t j = 0; j < run_length && output.size < MAX_OUTPUT_SIZE; j++) {
                output.data[output.size++] = value;
            }
            i += 3;
        } else {
            output.data[output.size++] = input[i++];
        }
    }
    
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t test_data[] = "AAAABBBCCCDDDEEEEFFFFGGGGHHHHIIIIJJJJ";
    size_t test_size = strlen((char*)test_data);
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", test_size);
    
    Buffer compressed = compress_rle(test_data, test_size);
    if (compressed.size == 0 || compressed.size >= MAX_OUTPUT_SIZE) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / test_size) * 100);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.size == 0 || decompressed.size >= MAX_OUTPUT_SIZE) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed.data[decompressed.size] = '\0';
    
    if (memcmp(test_data, decompressed.data, test_size) == 0) {
        printf("Decompression successful: %s\n", decompressed.data);
    } else {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Processing user input...\n");
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char user_input[MAX_INPUT_SIZE];
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    size_t user_size = strlen(user_input);
    if (user_size > 0 && user_input[user_size - 1] == '\n') {
        user_input[user_size - 1] = '\0';
        user_size--;
    }
    
    if (user_size == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    Buffer user_compressed = compress_rle((uint8_t*)user_input, user_size);
    if (user_compressed.size == 0 || user_compressed.size >= MAX_OUTPUT_SIZE) {
        fprintf(stderr, "User input compression failed\n");
        return 1;
    }
    
    Buffer user_decompressed = decompress_rle(user_compressed.data, user_compressed.size);
    if (user_decompressed.size == 0 || user_decompressed.size >= MAX_OUTPUT_SIZE) {
        fprintf(stderr, "User input decompression failed\n