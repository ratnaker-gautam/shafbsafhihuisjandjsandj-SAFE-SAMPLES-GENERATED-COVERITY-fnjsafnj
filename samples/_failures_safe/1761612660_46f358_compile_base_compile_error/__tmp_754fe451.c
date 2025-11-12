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
        
        if (out_idx + 2 >= output_size) {
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
        output[i] = input[i] - input[i-1];
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

int compress_data(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    if (type >= COMPRESS_TYPE_COUNT) {
        return -1;
    }
    
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

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    
    printf("Enter up to 256 bytes of data (hex values, space separated): ");
    
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_len < sizeof(input_data)) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit((unsigned char)token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            fprintf(stderr, "Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned int value;
        if (sscanf(token, "%02x", &value) != 1) {
            fprintf(stderr, "Failed to parse hex: %s\n", token);
            return 1;
        }
        
        input_data[input_len++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("Select compression type (0=none, 1=RLE, 2=delta): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice >= COMPRESS_TYPE_COUNT) {
        fprintf(stderr, "Invalid compression type\n");
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)choice;
    
    int result = compress_data(comp_type, input_data, input_len, output_data, sizeof(output_data));
    
    if (result < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%d bytes): ", result);
    for (int i = 0; i < result; i++) {