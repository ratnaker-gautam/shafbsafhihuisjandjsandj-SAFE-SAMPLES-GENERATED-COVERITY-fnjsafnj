//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if (size > max || size < 0) { return EXIT_FAILURE; }
#define SAFE_ADD(a, b) ((a) > SIZE_MAX - (b) ? SIZE_MAX : (a) + (b))

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || !input || !output) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_len = (i + CHUNK_SIZE <= len) ? CHUNK_SIZE : len - i;
        memcpy(chunk, input + i, chunk_len);
        for (size_t j = 0; j < chunk_len; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) return out_idx;
            output[out_idx++] = (chunk[j] >> SHIFT_AMOUNT) | ((chunk[j] & 0x0F) << SHIFT_AMOUNT);
        }
    }
    return out_idx;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || !input || !output) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (out_idx >= MAX_OUTPUT_SIZE) return out_idx;
        output[out_idx++] = (input[i] >> SHIFT_AMOUNT) | ((input[i] & 0x0F) << SHIFT_AMOUNT);
    }
    return out_idx;
}

static int read_input(Buffer* buf) {
    if (!buf) return EXIT_FAILURE;
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char temp[MAX_INPUT_SIZE * 2];
    if (!fgets(temp, sizeof(temp), stdin)) return EXIT_FAILURE;
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') temp[--len] = '\0';
    CHECK_BOUNDS(len, MAX_INPUT_SIZE);
    buf->size = len;
    memcpy(buf->data, temp, len);
    return EXIT_SUCCESS;
}

static void print_hex(const uint8_t* data, size_t len) {
    if (!data || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};

    if (read_input(&input) != EXIT_SUCCESS) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }

    int comp_size = compress_chunk(input.data, input.size, compressed.data);
    if (comp_size <= 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    compressed.size = comp_size;

    int decomp_size = decompress_chunk(compressed.data, compressed.size, decompressed.data);
    if (decomp_size <= 0 || decomp_size != input.size) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    decompressed.size = decomp_size;

    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Data mismatch\n");
        return EXIT_FAILURE;
    }

    printf("Original: ");
    print_hex(input.data, input.size);
    printf("Compressed: ");
    print_hex(compressed.data, compressed.size);
    printf("Decompressed: ");
    print_hex(decompressed.data, decompressed.size);
    printf("Compression ratio: %.2f\n", (float)compressed.size / input.size);

    return EXIT_SUCCESS;
}