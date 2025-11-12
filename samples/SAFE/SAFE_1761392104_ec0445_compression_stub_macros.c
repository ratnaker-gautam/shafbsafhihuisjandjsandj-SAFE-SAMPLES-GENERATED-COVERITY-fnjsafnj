//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 8
#define VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!VALID_CHAR(input[i])) {
            return 0;
        }
    }
    return 1;
}

Buffer compress_stub(const char *input, size_t len) {
    Buffer result = {NULL, 0};
    if (!validate_input(input, len)) {
        return result;
    }

    size_t compressed_size = len * COMPRESSED_MULTIPLIER;
    if (compressed_size > SIZE_MAX / sizeof(uint8_t)) {
        return result;
    }

    result.data = malloc(compressed_size * sizeof(uint8_t));
    if (result.data == NULL) {
        return result;
    }
    result.size = compressed_size;

    for (size_t i = 0; i < len; i++) {
        uint16_t encoded = (uint16_t)input[i] << SHIFT_AMOUNT;
        result.data[i * 2] = (encoded >> SHIFT_AMOUNT) & BYTE_MASK;
        result.data[i * 2 + 1] = encoded & BYTE_MASK;
    }

    return result;
}

Buffer decompress_stub(const uint8_t *input, size_t len) {
    Buffer result = {NULL, 0};
    if (input == NULL || len == 0 || len % 2 != 0 || len > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
        return result;
    }

    size_t decompressed_size = len / 2;
    if (decompressed_size > SIZE_MAX / sizeof(uint8_t)) {
        return result;
    }

    result.data = malloc(decompressed_size * sizeof(uint8_t));
    if (result.data == NULL) {
        return result;
    }
    result.size = decompressed_size;

    for (size_t i = 0; i < decompressed_size; i++) {
        uint16_t encoded = (input[i * 2] << SHIFT_AMOUNT) | input[i * 2 + 1];
        result.data[i] = (encoded >> SHIFT_AMOUNT) & BYTE_MASK;
        if (!VALID_CHAR(result.data[i])) {
            free(result.data);
            result.data = NULL;
            result.size = 0;
            return result;
        }
    }

    return result;
}

void print_buffer(const Buffer *buf, const char *label) {
    if (buf == NULL || buf->data == NULL) {
        printf("%s: [Invalid]\n", label);
        return;
    }
    printf("%s: ", label);
    for (size_t i = 0; i < buf->size; i++) {
        printf("%c", buf->data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (!validate_input(input, len)) {
        printf("Invalid input characters\n");
        return 1;
    }

    Buffer compressed = compress_stub(input, len);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }

    Buffer decompressed = decompress_stub(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }

    print_buffer(&compressed, "Compressed");
    print_buffer(&decompressed, "Decompressed");

    int match = (len == decompressed.size) && (memcmp(input, decompressed.data, len) == 0);
    printf("Round-trip successful: %s\n", match ? "Yes" : "No");

    free(compressed.data);
    free(decompressed.data);
    return 0;
}