//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_COMPRESSED_SIZE];
    size_t input_length = 0;
    size_t compressed_length = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int current_char;
    while ((current_char = getchar()) != EOF && current_char != '\n') {
        CHECK_BOUNDS(input_length, MAX_INPUT_SIZE);
        input_buffer[input_length] = (uint8_t)current_char;
        input_length++;
    }
    
    if (input_length == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_length; i++) {
        CHECK_BOUNDS(compressed_length, MAX_COMPRESSED_SIZE);
        compressed_buffer[compressed_length] = input_buffer[i] >> SHIFT_AMOUNT;
        compressed_length++;
        
        CHECK_BOUNDS(compressed_length, MAX_COMPRESSED_SIZE);
        compressed_buffer[compressed_length] = input_buffer[i] & BYTE_MASK;
        compressed_length++;
    }
    
    printf("Original size: %zu bytes\n", input_length);
    printf("Compressed size: %zu bytes\n", compressed_length);
    printf("Compression ratio: %.2f\n", (double)compressed_length / input_length);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_length; i++) {
        printf("%02X", compressed_buffer[i]);
    }
    printf("\n");
    
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    size_t decompressed_length = 0;
    
    for (size_t i = 0; i < compressed_length; i += 2) {
        if (i + 1 >= compressed_length) {
            break;
        }
        CHECK_BOUNDS(decompressed_length, MAX_INPUT_SIZE);
        decompressed_buffer[decompressed_length] = (compressed_buffer[i] << SHIFT_AMOUNT) | compressed_buffer[i + 1];
        decompressed_length++;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_length; i++) {
        putchar(decompressed_buffer[i]);
    }
    printf("\n");
    
    if (decompressed_length == input_length && memcmp(input_buffer, decompressed_buffer, input_length) == 0) {
        printf("Decompression verified successfully.\n");
    } else {
        printf("Decompression verification failed.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}