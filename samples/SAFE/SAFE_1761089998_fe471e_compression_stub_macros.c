//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static size_t safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    for (size_t i = 0; i < max_len; i++) {
        if (str[i] == '\0') return i;
    }
    return max_len;
}

static int validate_buffer(const void *buf, size_t size) {
    return (buf != NULL && size > 0 && size <= MAX_INPUT_SIZE);
}

static void simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!validate_buffer(input, input_len) || output == NULL || output_len == NULL) {
        if (output_len) *output_len = 0;
        return;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < MAX_OUTPUT_SIZE - 2; i += CHUNK_SIZE) {
        uint8_t count = 1;
        uint8_t value = input[i];
        
        for (size_t j = 1; j < CHUNK_SIZE && i + j < input_len; j++) {
            if (input[i + j] == value && count < UINT8_MAX) {
                count++;
            } else {
                break;
            }
        }
        
        if (out_idx + 2 <= MAX_OUTPUT_SIZE) {
            output[out_idx++] = count;
            output[out_idx++] = value;
        }
    }
    
    *output_len = out_idx;
}

static int simple_decompress(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!validate_buffer(input, input_len) || output == NULL || output_len == NULL) {
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i + 1 < input_len && out_idx < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            count = MAX_OUTPUT_SIZE - out_idx;
        }
        
        for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_len = out_idx;
    return 1;
}

int main(void) {
    char user_input[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(user_input, sizeof(user_input));
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, user_input, input_len);
    
    size_t compressed_len = 0;
    simple_compress(input_data, input_len, compressed, &compressed_len);
    
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f:1\n", (double)input_len / compressed_len);
    
    size_t decompressed_len = 0;
    if (!simple_decompress(compressed, compressed_len, decompressed, &decompressed_len)) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_len != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed successfully: ");
    for (size_t i = 0; i < decompressed_len && i < 50; i++) {
        putchar(decompressed[i]);
    }
    if (decompressed_len > 50) printf("...");
    putchar('\n');
    
    return EXIT_SUCCESS;
}