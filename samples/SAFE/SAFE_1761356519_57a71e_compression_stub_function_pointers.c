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
        
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
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
    
    size_t max_output = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (input->data[i] == 0) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        max_output += input->data[i];
    }
    
    uint8_t* output_data = malloc(max_output);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_index + count > max_output) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output_data[out_index++] = value;
        }
    }
    
    Buffer result = {output_data, out_index};
    return result;
}

void free_buffer(Buffer* buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    CompressFunc compressors[] = {compress_rle};
    DecompressFunc decompressors[] = {decompress_rle};
    const char* names[] = {"RLE"};
    size_t num_methods = 1;
    
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    Buffer input = {test_data, sizeof(test_data)};
    
    printf("Original data (%zu bytes): ", input.size);
    for (size_t i = 0; i < input.size; i++) {
        printf("%d ", input.data[i]);
    }
    printf("\n");
    
    for (size_t i = 0; i < num_methods; i++) {
        printf("\nTesting %s compression:\n", names[i]);
        
        Buffer compressed = compressors[i](&input);
        if (compressed.data == NULL) {
            printf("Compression failed\n");
            continue;
        }
        
        printf("Compressed data (%zu bytes): ", compressed.size);
        for (size_t j = 0; j < compressed.size; j++) {
            printf("%d ", compressed.data[j]);
        }
        printf("\n");
        
        Buffer decompressed = decompressors[i](&compressed);
        if (decompressed.data == NULL) {
            printf("Decompression failed\n");
            free_buffer(&compressed);
            continue;
        }
        
        printf("Decompressed data (%zu bytes): ", decompressed.size);
        for (size_t j = 0; j < decompressed.size; j++) {
            printf("%d ", decompressed.data[j]);
        }
        printf("\n");
        
        if (decompressed.size == input.size && 
            memcmp(decompressed.data, input.data, input.size) == 0) {
            printf("Verification: SUCCESS\n");
        } else {
            printf("Verification: FAILED\n");
        }
        
        free_buffer(&compressed);
        free_buffer(&decompressed);
    }
    
    return 0;
}