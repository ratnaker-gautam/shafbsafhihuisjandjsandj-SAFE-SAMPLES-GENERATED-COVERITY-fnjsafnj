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
        if (i >= output_size) {
            return -1;
        }
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

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    char buffer[256];
    int method_choice;
    
    printf("Enter up to 256 bytes of data (hex values, space separated): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    char *token = strtok(buffer, " \n");
    while (token != NULL && input_len < 256) {
        if (strlen(token) != 2) {
            printf("Invalid hex byte: %s\n", token);
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
            printf("Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned int value;
        if (sscanf(token, "%02x", &value) != 1) {
            printf("Failed to parse hex: %s\n", token);
            return 1;
        }
        
        if (value > 255) {
            printf("Value out of range: %s\n", token);
            return 1;
        }
        
        input_data[input_len++] = (uint8_t)value;
        token = strtok(NULL, " \n");
    }
    
    if (input_len == 0) {
        printf("No valid input data provided\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("0: No compression\n");
    printf("1: Run-length encoding\n");
    printf("2: Delta encoding\n");
    printf("Choice: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    if (sscanf(buffer, "%d", &method_choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (method_choice < 0 || method_choice >= COMPRESS_TYPE_COUNT) {
        printf("Invalid compression method\n");
        return 1;
    }
    
    int result_size;
    switch (method_choice) {
        case COMPRESS_NONE:
            result_size = compress_none(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_RLE:
            result_size = compress_rle(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_DELTA:
            result_size = compress_delta(input_data, input_len, output_data, sizeof(output_data));
            break;
        default:
            printf("Invalid compression method\n");
            return 1;
    }
    
    if (result_size < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", result_size);
    printf("Compression ratio: %.2f%%\n", (float)