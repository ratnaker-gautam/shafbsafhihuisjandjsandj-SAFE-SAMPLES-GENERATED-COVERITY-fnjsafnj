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
} CompressedData;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (!validate_input(input, input_size) || output == NULL) {
        return 0;
    }

    size_t output_index = 0;
    size_t i = 0;

    while (i < input_size) {
        if (output_index >= MAX_OUTPUT_SIZE - 2) {
            return 0;
        }

        uint8_t current = input[i];
        size_t count = 1;

        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }

        output->data[output_index++] = current;
        output->data[output_index++] = (uint8_t)count;
        i += count;
    }

    output->size = output_index;
    return output_index;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0) return 0;
    if (compressed->size % 2 != 0) return 0;

    size_t output_index = 0;
    size_t i = 0;

    while (i < compressed->size) {
        uint8_t value = compressed->data[i++];
        uint8_t count = compressed->data[i++];

        if (output_index + count > output_size) {
            return 0;
        }

        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }

    return output_index;
}

int compare_data(const uint8_t* data1, const uint8_t* data2, size_t size) {
    if (data1 == NULL || data2 == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (data1[i] != data2[i]) return 0;
    }
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i < size - 1) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes as hexadecimal values: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid input data\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    size_t compressed_size = simple_compress(input_data, input_size, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed_size / input_size * 100.0f);
    
    size_t decompressed_size = simple_decompress(&compressed, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed_data, decompressed_size);
    
    if (decompressed_size == input_size && 
        compare_data(input_data, decompressed_data, input_size)) {
        printf("\nVerification: SUCCESS\n");
    } else {
        printf("\nVerification: FAILED\n");
        return 1;
    }
    
    return 0;
}