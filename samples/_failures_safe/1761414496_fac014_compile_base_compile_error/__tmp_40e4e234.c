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
    
    size_t max_output = input->size * 2 + 1;
    if (max_output < input->size) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t* output_data = malloc(max_output);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
            count++;
        }
        
        if (output_index + 2 > max_output) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        output_data[output_index++] = (uint8_t)count;
        output_data[output_index++] = current;
        i += count;
    }
    
    Buffer result = {output_data, output_index};
    return result;
}

Buffer decompress_rle(const Buffer* input) {
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t output_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (input->data[i] > UINT8_MAX - output_size) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        output_size += input->data[i];
    }
    
    if (output_size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t* output_data = malloc(output_size);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (output_index + count > output_size) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output_data[output_index++] = value;
        }
    }
    
    Buffer result = {output_data, output_size};
    return result;
}

Buffer compress_null(const Buffer* input) {
    if (input == NULL || input->data == NULL || input->size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t* output_data = malloc(input->size);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    memcpy(output_data, input->data, input->size);
    Buffer result = {output_data, input->size};
    return result;
}

Buffer decompress_null(const Buffer* input) {
    return compress_null(input);
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const char* name) {
    printf("Testing %s compression\n", name);
    
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    Buffer input = {test_data, sizeof(test_data)};
    
    printf("Original: ");
    for (size_t i = 0; i < input.size; i++) {
        printf("%d ", input.data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress(&input);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    int match = (decompressed.size == input.size) && 
                (memcmp(decompressed.data, input.data, input.size) == 0);
    printf("Round-trip successful: %s\n", match ? "yes" : "no");
    
    free(compressed.data);
    free(decompressed.data);
    printf("\