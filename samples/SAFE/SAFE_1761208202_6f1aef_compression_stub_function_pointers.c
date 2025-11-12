//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
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
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t output_size = func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (output_size > 0 && output_size <= MAX_OUTPUT_SIZE) {
        result.data = output;
        result.size = output_size;
    } else {
        free(output);
    }
    
    return result;
}

void print_buffer(const Buffer *buf, const char *label) {
    if (buf == NULL || buf->data == NULL || label == NULL) return;
    
    printf("%s (%zu bytes): ", label, buf->size);
    for (size_t i = 0; i < buf->size && i < 32; i++) {
        printf("%02X ", buf->data[i]);
    }
    if (buf->size > 32) printf("...");
    printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    CompressFunc compressors[] = {run_length_encode};
    DecompressFunc decompressors[] = {run_length_decode};
    const char *names[] = {"Run-Length Encoding"};
    size_t num_methods = 1;
    
    printf("Original data (%zu bytes): ", test_size);
    for (size_t i = 0; i < test_size; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\n\n");
    
    for (size_t i = 0; i < num_methods; i++) {
        printf("Method: %s\n", names[i]);
        
        Buffer compressed = process_data(test_data, test_size, compressors[i]);
        if (compressed.data != NULL) {
            print_buffer(&compressed, "Compressed");
            
            Buffer decompressed = process_data(compressed.data, compressed.size, (CompressFunc)decompressors[i]);
            if (decompressed.data != NULL) {
                print_buffer(&decompressed, "Decompressed");
                
                if (decompressed.size == test_size && 
                    memcmp(decompressed.data, test_data, test_size) == 0) {
                    printf("Verification: SUCCESS\n");
                } else {
                    printf("Verification: FAILED\n");
                }
                
                free(decompressed.data);
            }
            
            free(compressed.data);
        }
        printf("\n");
    }
    
    return 0;
}