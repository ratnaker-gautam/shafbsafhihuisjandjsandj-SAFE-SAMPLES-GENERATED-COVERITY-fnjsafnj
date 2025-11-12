//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR '\\'
#define MAX_RUN_LENGTH 255

#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)
#define SAFE_INCREMENT(cnt, max) do { if ((cnt) >= (max)) return -1; (cnt)++; } while (0)

static int compress_rle(const char *input, size_t input_len, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) return -1;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (out_idx >= output_size) return -1;
        
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < MAX_RUN_LENGTH && input[i + count] == current) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_idx + 3 >= output_size) return -1;
            output[out_idx++] = ESCAPE_CHAR;
            output[out_idx++] = count;
            output[out_idx++] = current;
        } else {
            for (uint8_t j = 0; j < count; j++) {
                if (out_idx >= output_size) return -1;
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    if (out_idx >= output_size) return -1;
    output[out_idx] = '\0';
    return (int)out_idx;
}

static int decompress_rle(const char *input, size_t input_len, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) return -1;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (out_idx >= output_size) return -1;
        
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) return -1;
            
            uint8_t count = (uint8_t)input[i + 1];
            char character = input[i + 2];
            
            if (!IS_PRINTABLE(character) || count == 0) return -1;
            
            if (out_idx + count > output_size) return -1;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = character;
            }
            
            i += 3;
        } else {
            if (!IS_PRINTABLE(input[i])) return -1;
            output[out_idx++] = input[i++];
        }
    }
    
    if (out_idx >= output_size) return -1;
    output[out_idx] = '\0';
    return (int)out_idx;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    char compressed[MAX_OUTPUT_SIZE];
    char decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!IS_PRINTABLE(input_buffer[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return EXIT_FAILURE;
        }
    }
    
    int compressed_size = compress_rle(input_buffer, input_len, compressed, sizeof(compressed));
    if (compressed_size < 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed (%d bytes): ", compressed_size);
    for (int i = 0; i < compressed_size; i++) {
        if (IS_PRINTABLE(compressed[i])) {
            putchar(compressed[i]);
        } else {
            printf("\\x%02X", (unsigned char)compressed[i]);
        }
    }
    printf("\n");
    
    int decompressed_size = decompress_rle(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size < 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed (%d bytes): %s\n", decompressed_size, decompressed);
    
    if (strcmp(input_buffer, decompressed) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Verification successful\n");
    return EXIT_SUCCESS;
}