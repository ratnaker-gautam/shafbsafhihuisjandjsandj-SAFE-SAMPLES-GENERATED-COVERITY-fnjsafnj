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
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 < output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    
    if (output_size > 0 && output_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(output_size);
        if (result.data != NULL) {
            memcpy(result.data, output_buffer, output_size);
            result.size = output_size;
        }
    }
    
    free(output_buffer);
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t input_size = sizeof(input_data);
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Original data (%zu bytes):\n", input_size);
    print_buffer_hex(input_data, input_size);
    
    Buffer compressed = process_data(input_data, input_size, compress_func);
    if (compressed.data != NULL && compressed.size > 0) {
        printf("\nCompressed data (%zu bytes):\n", compressed.size);
        print_buffer_hex(compressed.data, compressed.size);
        
        Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompress_func);
        if (decompressed.data != NULL && decompressed.size > 0) {
            printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
            print_buffer_hex(decompressed.data, decompressed.size);
            
            if (decompressed.size == input_size && 
                memcmp(input_data, decompressed.data, input_size) == 0) {
                printf("\nVerification: SUCCESS - Data matches original\n");
            } else {
                printf("\nVerification: FAILED - Data does not match original\n");
            }
            
            free(decompressed.data);
        } else {
            printf("\nDecompression failed\n");
        }
        
        free(compressed.data);
    } else {
        printf("\nCompression failed\n");
    }
    
    return 0;
}