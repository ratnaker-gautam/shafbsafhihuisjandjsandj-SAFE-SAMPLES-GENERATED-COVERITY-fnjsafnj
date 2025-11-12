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
#define VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static Buffer compress_data(const char *input, size_t len) {
    Buffer result = {NULL, 0};
    if (len == 0 || len > SIZE_MAX / COMPRESSED_MULTIPLIER) return result;
    
    size_t compressed_size = len * COMPRESSED_MULTIPLIER;
    if (compressed_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) return result;
    
    result.data = malloc(compressed_size);
    if (result.data == NULL) return result;
    result.size = compressed_size;
    
    for (size_t i = 0; i < len; i++) {
        uint8_t current = (uint8_t)input[i];
        result.data[i * 2] = (current >> SHIFT_AMOUNT) & BYTE_MASK;
        result.data[i * 2 + 1] = current & BYTE_MASK;
    }
    
    return result;
}

static Buffer decompress_data(const uint8_t *compressed, size_t len) {
    Buffer result = {NULL, 0};
    if (compressed == NULL) return result;
    if (len == 0 || len % 2 != 0) return result;
    if (len > SIZE_MAX / 2) return result;
    
    size_t original_size = len / 2;
    if (original_size > MAX_INPUT_SIZE) return result;
    
    result.data = malloc(original_size);
    if (result.data == NULL) return result;
    result.size = original_size;
    
    for (size_t i = 0; i < original_size; i++) {
        uint8_t high = compressed[i * 2];
        uint8_t low = compressed[i * 2 + 1];
        if (high > 15 || low > 15) {
            free(result.data);
            result.data = NULL;
            result.size = 0;
            return result;
        }
        result.data[i] = (high << SHIFT_AMOUNT) | low;
        if (!VALID_CHAR(result.data[i])) {
            free(result.data);
            result.data = NULL;
            result.size = 0;
            return result;
        }
    }
    
    return result;
}

static void print_buffer(const Buffer *buf, const char *label) {
    if (buf == NULL || buf->data == NULL) return;
    printf("%s: ", label);
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input);
    
    Buffer compressed = compress_data(input, len);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    print_buffer(&compressed, "Compressed");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    int verify = (decompressed.size == len) && 
                 (memcmp(input, decompressed.data, len) == 0);
    printf("Verification: %s\n", verify ? "PASS" : "FAIL");
    
    free(compressed.data);
    free(decompressed.data);
    
    return verify ? EXIT_SUCCESS : EXIT_FAILURE;
}