//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_DELTA,
    COMPRESS_TYPE_COUNT
};

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_size - 2) {
        uint8_t current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) {
            return -1;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return (int)out_idx;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    if (output_size < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    for (size_t i = 1; i < input_len; i++) {
        if (i >= output_size) {
            return -1;
        }
        int16_t delta = (int16_t)input[i] - (int16_t)input[i-1];
        if (delta < -128 || delta > 127) {
            return -1;
        }
        output[i] = (uint8_t)(delta + 128);
    }
    
    return (int)input_len;
}

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    
    memcpy(output, input, input_len);
    return (int)input_len;
}

int perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    switch (type) {
        case COMPRESS_NONE:
            return compress_none(input, input_len, output, output_size);
        case COMPRESS_RLE:
            return compress_rle(input, input_len, output, output_size);
        case COMPRESS_DELTA:
            return compress_delta(input, input_len, output, output_size);
        default:
            return -1;
    }
}

void print_compression_types(void) {
    printf("Available compression types:\n");
    printf("0: No compression\n");
    printf("1: Run-length encoding\n");
    printf("2: Delta encoding\n");
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    char input_buffer[4];
    int compression_choice;
    
    printf("Enter up to 256 bytes of data (0-255, space separated): ");
    
    for (int i = 0; i < 256; i++) {
        if (scanf("%3s", input_buffer) != 1) {
            break;
        }
        
        char *endptr;
        long value = strtol(input_buffer, &endptr, 10);
        if (endptr == input_buffer || *endptr != '\0') {
            break;
        }
        
        if (value < 0 || value > 255) {
            break;
        }
        
        input_data[input_len++] = (uint8_t)value;
        
        int next_char = getchar();
        if (next_char == '\n' || next_char == EOF) {
            break;
        }
    }
    
    if (input_len == 0) {
        printf("No valid input data provided.\n");
        return 1;
    }
    
    print_compression_types();
    printf("Select compression type (0-2): ");
    
    if (scanf("%d", &compression_choice) != 1) {
        printf("Invalid compression type selection.\n");
        return 1;
    }
    
    if (compression_choice < 0 || compression_choice >= COMPRESS_TYPE_COUNT) {
        printf("Invalid compression type.\n");
        return 1;
    }
    
    int result = perform_compression((enum compression_type)compression_choice, input_data, input_len, output_data, sizeof(output_data));
    
    if (result < 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len && i < 20; i++) {
        printf("%d ", input_data[i]);
    }
    if (input_len > 20) {
        printf("...");
    }
    printf("\n");
    
    printf("Compressed data (%d bytes): ", result);
    for