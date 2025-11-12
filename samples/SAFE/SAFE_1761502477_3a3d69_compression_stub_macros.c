//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255
#define MIN_RUN_LENGTH 3

typedef struct {
    uint8_t data[MAX_INPUT_SIZE * 2];
    size_t size;
} CompressedData;

size_t compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }

    size_t out_idx = 0;
    size_t i = 0;

    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;

        while (i + run_length < input_size && run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }

        if (run_length >= MIN_RUN_LENGTH) {
            if (out_idx + 2 >= sizeof(output->data)) {
                return 0;
            }
            output->data[out_idx++] = COMPRESSED_MARKER;
            output->data[out_idx++] = (uint8_t)run_length;
            output->data[out_idx++] = current;
            i += run_length;
        } else {
            if (out_idx + 1 >= sizeof(output->data)) {
                return 0;
            }
            output->data[out_idx++] = current;
            i++;
        }
    }

    output->size = out_idx;
    return out_idx;
}

size_t decompress_rle(const CompressedData* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size == 0 || output_size < MAX_INPUT_SIZE) {
        return 0;
    }

    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input->size) {
        if (input->data[in_idx] == COMPRESSED_MARKER) {
            if (in_idx + 2 >= input->size) {
                return 0;
            }
            uint8_t run_length = input->data[in_idx + 1];
            uint8_t value = input->data[in_idx + 2];

            if (out_idx + run_length > output_size) {
                return 0;
            }

            for (uint8_t j = 0; j < run_length; j++) {
                output[out_idx++] = value;
            }
            in_idx += 3;
        } else {
            if (out_idx >= output_size) {
                return 0;
            }
            output[out_idx++] = input->data[in_idx++];
        }
    }

    return out_idx;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }

    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    memset(&compressed, 0, sizeof(compressed));
    size_t compressed_size = compress_rle(input_data, input_len, &compressed);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }

    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_len) * 100.0);

    memset(decompressed, 0, sizeof(decompressed));
    size_t decompressed_size = decompress_rle(&compressed, decompressed, sizeof(decompressed));
    
    if (decompressed_size != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }

    printf("Decompression verified successfully\n");
    printf("Decompressed text: %s\n", decompressed);

    return 0;
}