//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE)

#define RLE_THRESHOLD 3
#define BYTE_MASK 0xFF
#define COMPRESSION_MAGIC 0x43505253

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(pos, max) if ((pos) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

static int compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!input || !output || input_size == 0 || output_size < HEADER_SIZE) {
        return -1;
    }

    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = COMPRESSION_MAGIC;
    header->original_size = (uint32_t)input_size;

    size_t out_pos = HEADER_SIZE;
    size_t in_pos = 0;

    while (in_pos < input_size) {
        uint8_t current = input[in_pos];
        size_t count = 1;

        while (in_pos + count < input_size && count < BYTE_MASK && input[in_pos + count] == current) {
            count++;
        }

        if (count >= RLE_THRESHOLD) {
            if (out_pos + 2 >= output_size) {
                return -1;
            }
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count; i++) {
                if (out_pos + 1 >= output_size) {
                    return -1;
                }
                output[out_pos++] = current;
            }
        }

        in_pos += count;
    }

    return (int)out_pos;
}

static int decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!input || !output || input_size < HEADER_SIZE) {
        return -1;
    }

    CompressionHeader *header = (CompressionHeader *)input;
    if (header->magic != COMPRESSION_MAGIC) {
        return -1;
    }

    if (header->original_size > output_size) {
        return -1;
    }

    size_t in_pos = HEADER_SIZE;
    size_t out_pos = 0;

    while (in_pos < input_size && out_pos < header->original_size) {
        if (in_pos + 1 >= input_size) {
            return -1;
        }

        uint8_t value = input[in_pos++];
        uint8_t next = input[in_pos];

        if (in_pos + 1 < input_size && next >= RLE_THRESHOLD) {
            size_t count = (size_t)next;
            if (out_pos + count > header->original_size) {
                return -1;
            }
            for (size_t i = 0; i < count; i++) {
                output[out_pos++] = value;
            }
            in_pos++;
        } else {
            if (out_pos + 1 > header->original_size) {
                return -1;
            }
            output[out_pos++] = value;
        }
    }

    if (out_pos != header->original_size) {
        return -1;
    }

    return (int)out_pos;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_COMPRESSED_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];

    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets((char *)input_buffer, MAX_INPUT_SIZE, stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t input_size = strlen((char *)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }

    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }

    int compressed_size = compress_rle(input_buffer, input_size, compressed_buffer, MAX_COMPRESSED_SIZE);
    if (compressed_size < 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %d bytes\n", compressed_size);

    int decompressed_size = decompress_rle(compressed_buffer, (size_t)compressed_size, decompressed_buffer, MAX_INPUT_SIZE);
    if (decompressed_size < 0 || (size_t)decompressed_size != input_size) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        fprintf(stderr, "Ver