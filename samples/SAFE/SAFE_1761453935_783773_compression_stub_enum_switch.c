//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_DELTA,
    COMPRESS_TYPE_COUNT
};

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || output_len < input_len) {
        return -1;
    }
    memcpy(output, input, input_len);
    return (int)input_len;
}

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len < 2) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 <= output_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    if (in_idx < input_len) {
        return -1;
    }
    
    return (int)out_idx;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    for (size_t i = 1; i < input_len && i < output_len; i++) {
        int16_t delta = (int16_t)input[i] - (int16_t)input[i-1];
        if (delta < -128 || delta > 127) {
            return -1;
        }
        output[i] = (uint8_t)(delta + 128);
    }
    
    return (int)input_len;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    
    printf("Enter data length (1-256): ");
    if (scanf("%zu", &input_len) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input_len < 1 || input_len > 256) {
        printf("Invalid length\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (value > 255) {
            printf("Value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    printf("Select compression type:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: Delta\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice >= COMPRESS_TYPE_COUNT) {
        printf("Invalid choice\n");
        return 1;
    }
    
    int result;
    enum compression_type comp_type = (enum compression_type)choice;
    
    switch (comp_type) {
        case COMPRESS_NONE:
            result = compress_none(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_RLE:
            result = compress_rle(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_DELTA:
            result = compress_delta(input_data, input_len, output_data, sizeof(output_data));
            break;
        default:
            printf("Invalid compression type\n");
            return 1;
    }
    
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed %zu bytes to %d bytes\n", input_len, result);
    printf("Compressed data: ");
    for (int i = 0; i < result; i++) {
        printf("%u ", output_data[i]);
    }
    printf("\n");
    
    return 0;
}