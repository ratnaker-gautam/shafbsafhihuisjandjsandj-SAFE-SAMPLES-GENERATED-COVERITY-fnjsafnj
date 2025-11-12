//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_BOUNDS(ptr, size, max) do { \
    if ((size) > (max) || (ptr) == NULL) { \
        return -1; \
    } \
} while(0)

#define VALIDATE_INPUT(input, len) do { \
    if ((input) == NULL || (len) == 0 || (len) > MAX_INPUT_SIZE) { \
        return -1; \
    } \
} while(0)

#define WRITE_HEADER(buf, magic, size) do { \
    uint32_t *header = (uint32_t *)(buf); \
    header[0] = (magic); \
    header[1] = (size); \
} while(0)

#define READ_HEADER(buf, magic, size) do { \
    const uint32_t *header = (const uint32_t *)(buf); \
    (magic) = header[0]; \
    (size) = header[1]; \
} while(0)

static int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);
    
    if (*output_len < HEADER_SIZE + (input_len + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR) {
        return -1;
    }
    
    WRITE_HEADER(output, MAGIC_NUMBER, (uint32_t)input_len);
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_len; j++) {
            compressed |= (input[i + j] & 0xC0) >> (2 * (COMPRESSION_FACTOR - 1 - j));
        }
        output[out_idx++] = compressed;
    }
    
    *output_len = out_idx;
    return 0;
}

static int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_INPUT_SIZE);
    
    if (input_len < HEADER_SIZE) {
        return -1;
    }
    
    uint32_t magic, original_size;
    READ_HEADER(input, magic, original_size);
    
    if (magic != MAGIC_NUMBER || original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    if (*output_len < original_size) {
        return -1;
    }
    
    size_t compressed_size = input_len - HEADER_SIZE;
    size_t expected_compressed = (original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_size != expected_compressed) {
        return -1;
    }
    
    for (size_t i = 0; i < original_size; i++) {
        size_t block_idx = i / COMPRESSION_FACTOR;
        size_t pos_in_block = i % COMPRESSION_FACTOR;
        uint8_t compressed = input[HEADER_SIZE + block_idx];
        uint8_t shift = 2 * (COMPRESSION_FACTOR - 1 - pos_in_block);
        output[i] = (compressed << shift) & 0xC0;
    }
    
    *output_len = original_size;
    return 0;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

static int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (hex string, max %d bytes): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {