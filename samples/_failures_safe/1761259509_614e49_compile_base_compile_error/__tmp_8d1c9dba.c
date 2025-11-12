//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) {
        return 0;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    if (input_size == 0 || output_size == 0) {
        return 0;
    }

    size_t output_index = 0;
    size_t i = 0;

    while (i < input_size && output_index < output_size - 2) {
        uint8_t current = input[i];
        size_t count = 1;

        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }

        if (output_index + 2 > output_size) {
            return 0;
        }

        output[output_index] = (uint8_t)count;
        output[output_index + 1] = current;
        output_index += 2;
        i += count;
    }

    return output_index;
}

int decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    if (input_size == 0 || output_size == 0) {
        return 0;
    }

    size_t output_index = 0;
    size_t i = 0;

    while (i < input_size - 1 && output_index < output_size) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];

        if (output_index + count > output_size) {
            return 0;
        }

        for (uint8_t j = 0; j < count; j++) {
            output[output_index + j] = value;
        }

        output_index += count;
        i += 2;
    }

    return 1;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) {
        return 0;
    }
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    size_t data_size = 0;
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid data size\n");
        return 1;
    }

    printf("Enter %zu bytes of data (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            printf("Value out of range\n");
            return 1;
        }
        original_data[i] = (uint8_t)value;
    }

    if (!validate_input(original_data, data_size)) {
        printf("Input validation failed\n");
        return 1;
    }

    size_t compressed_size = compress_rle(original_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }

    printf("Original size: %zu\n", data_size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", (float)data_size / compressed_size);

    memset(decompressed_data, 0, sizeof(decompressed_data));
    if (!decompress_rle(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE)) {
        printf("Decompression failed\n");
        return 1;
    }

    if (!compare_buffers(original_data, decompressed_data, data_size)) {
        printf("Verification failed\n");
        return 1;
    }

    printf("Compression and decompression successful\n");
    printf("Original data: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%d ", original_data[i]);
    }
    printf("\nCompressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\nDecompressed data: ");
    for (size_t i = 0; i < data_size; i++)