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
#define MAGIC_HEADER 0xABCD1234

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) {
        return -1;
    }
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        size_t remaining = MIN(COMPRESSION_RATIO, len - i);
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < remaining; j++) {
            compressed_byte |= (input[i + j] & 0xF0) >> (4 * j);
        }
        output[i / COMPRESSION_RATIO] = compressed_byte;
    }
    return len / COMPRESSION_RATIO + (len % COMPRESSION_RATIO ? 1 : 0);
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t original_len) {
    if (len == 0 || input == NULL || output == NULL || original_len > MAX_INPUT_SIZE) {
        return -1;
    }
    for (size_t i = 0; i < original_len; i++) {
        size_t input_idx = i / COMPRESSION_RATIO;
        if (input_idx >= len) {
            return -1;
        }
        uint8_t shift = 4 * (i % COMPRESSION_RATIO);
        output[i] = (input[input_idx] & (0xF0 >> shift)) << shift;
    }
    return original_len;
}

static int perform_compression(const uint8_t *input, size_t input_len, compressed_data_t *compressed) {
    if (input == NULL || compressed == NULL || input_len > MAX_INPUT_SIZE) {
        return -1;
    }
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint16_t)input_len;
    int compressed_size = compress_chunk(input, input_len, compressed->data);
    if (compressed_size < 0) {
        return -1;
    }
    compressed->compressed_size = (uint16_t)compressed_size;
    return 0;
}

static int perform_decompression(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    if (compressed == NULL || output == NULL) {
        return -1;
    }
    if (compressed->header != MAGIC_HEADER) {
        return -1;
    }
    if (compressed->original_size > output_size || compressed->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    int result = decompress_chunk(compressed->data, compressed->compressed_size, output, compressed->original_size);
    if (result != compressed->original_size) {
        return -1;
    }
    return 0;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE];
    compressed_data_t compressed;

    printf("Enter data to compress (hex string, max %d bytes): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[hex_len - 1] = '\0';
        hex_len--;
    }
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        return EXIT_FAILURE;
    }
    size_t input_len = hex_len / 2;
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = { hex_input[i * 2], hex_input[i * 2 + 1], '\0' };
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr == hex_byte || *endptr != '\0' || value < 0 || value > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_buffer[i] = (uint8_t)value;
    }

    if (perform_compression(input_buffer, input