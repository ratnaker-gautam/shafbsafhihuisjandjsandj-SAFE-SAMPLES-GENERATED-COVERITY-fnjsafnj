//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

typedef Buffer (*CompressFunc)(const Buffer* input);
typedef Buffer (*DecompressFunc)(const Buffer* input);

Buffer compress_rle(const Buffer* input) {
    if (input == NULL || input->data == NULL || input->size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t max_output = input->size * 2 + 2;
    uint8_t* output_data = malloc(max_output);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_index + 2 > max_output) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        output_data[out_index++] = (uint8_t)count;
        output_data[out_index++] = current;
        i += count;
    }
    
    Buffer result = {output_data, out_index};
    return result;
}

Buffer decompress_rle(const Buffer* input) {
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t output_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (input->data[i] == 0) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        if (output_size > SIZE_MAX - input->data[i]) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        output_size += input->data[i];
    }
    
    uint8_t* output_data = malloc(output_size);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (out_index >= output_size) {
                free(output_data);
                Buffer empty = {NULL, 0};
                return empty;
            }
            output_data[out_index++] = value;
        }
    }
    
    Buffer result = {output_data, output_size};
    return result;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    Buffer input = {test_data, sizeof(test_data)};
    
    printf("Original data: ");
    for (size_t i = 0; i < input.size; i++) {
        printf("%d ", input.data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress(&input);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f\n", (double)compressed.size / input.size);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input.size && memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compress_func = compress_rle;
    DecompressFunc decompress_func = decompress_rle;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}