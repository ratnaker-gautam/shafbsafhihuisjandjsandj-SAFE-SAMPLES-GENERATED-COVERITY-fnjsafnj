//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer process_input(CompressFunc compress, DecompressFunc decompress, const uint8_t *input, size_t input_size, int mode) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    uint8_t *temp_buffer = malloc(MAX_OUTPUT_SIZE);
    if (temp_buffer == NULL) {
        return result;
    }
    
    size_t processed_size;
    
    if (mode == 0) {
        processed_size = compress(input, input_size, temp_buffer, MAX_OUTPUT_SIZE);
    } else {
        processed_size = decompress(input, input_size, temp_buffer, MAX_OUTPUT_SIZE);
    }
    
    if (processed_size > 0 && processed_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(processed_size);
        if (result.data != NULL) {
            memcpy(result.data, temp_buffer, processed_size);
            result.size = processed_size;
        }
    }
    
    free(temp_buffer);
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    CompressFunc compress_funcs[] = {run_length_encode};
    DecompressFunc decompress_funcs[] = {run_length_decode};
    const char *func_names[] = {"Run-Length Encoding"};
    
    int num_funcs = sizeof(compress_funcs) / sizeof(compress_funcs[0]);
    
    printf("Available compression methods:\n");
    for (int i = 0; i < num_funcs; i++) {
        printf("%d. %s\n", i + 1, func_names[i]);
    }
    
    printf("Select method (1-%d): ", num_funcs);
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_funcs) {
        printf("Invalid selection\n");
        return 1;
    }
    
    printf("Enter operation (0=compress, 1=decompress): ");
    int operation;
    if (scanf("%d", &operation) != 1 || (operation != 0 && operation != 1)) {
        printf("Invalid operation\n");
        return 1;
    }
    
    printf("Enter input data (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    unsigned int byte_val;
    
    while (input_size < MAX_INPUT_SIZE && scanf("%02X", &byte_val) == 1) {
        if (byte_val > 0xFF) {
            printf("Invalid byte value\n");
            return 1;
        }
        input_buffer[input_size++] = (uint8_t