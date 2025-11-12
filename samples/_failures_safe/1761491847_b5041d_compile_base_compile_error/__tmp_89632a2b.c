//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCDEF00

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (!input || !output || len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output, size_t orig_len) {
    if (!input || !output || len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (out_idx < orig_len) {
                CHECK_BOUNDS(out_idx, MAX_INPUT_SIZE);
                output[out_idx++] = (compressed >> (j * 2)) & 0x03;
            }
        }
    }
    return out_idx;
}

static int perform_compression(const uint8_t* input, size_t input_len, compressed_data_t* result) {
    if (!input || !result || input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    result->header = MAGIC_HEADER;
    result->original_size = input_len;
    int compressed_size = compress_chunk(input, input_len, result->data);
    if (compressed_size <= 0) return 0;
    result->compressed_size = compressed_size;
    return 1;
}

static int perform_decompression(const compressed_data_t* compressed, uint8_t* output) {
    if (!compressed || !output) return 0;
    if (compressed->header != MAGIC_HEADER) return 0;
    if (compressed->original_size > MAX_INPUT_SIZE) return 0;
    if (compressed->compressed_size > MAX_OUTPUT_SIZE) return 0;
    int result = decompress_chunk(compressed->data, compressed->compressed_size, output, compressed->original_size);
    return result == compressed->original_size;
}

static void print_hex(const uint8_t* data, size_t len) {
    if (!data || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % CHUNK_SIZE == 0) printf("\n");
        else if (i + 1 < len) printf(" ");
    }
    if (len % CHUNK_SIZE != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE * 2);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (!fgets(hex_input, sizeof(hex_input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = hex_len / 2;
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char* endptr;
        long byte_val = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || byte_val < 0 || byte_val > 255) {
            fprintf(stderr, "Invalid hex byte: %s\n", hex_byte