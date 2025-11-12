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
    if (max_output < input->size * 2) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
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
    
    size_t max_output = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (input->data[i] == 0) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        if (max_output + input->data[i] < max_output) {
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
        
        if (out_index + count > max_output || out_index + count < out_index) {
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

void process_buffer(const Buffer* input, CompressFunc compress, DecompressFunc decompress) {
    if (input == NULL || compress == NULL || decompress == NULL) {
        return;
    }
    
    Buffer compressed = compress(input);
    if (compressed.data == NULL) {
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input->size, compressed.size);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input->size && memcmp(decompressed.data, input->data, input->size) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compressors[] = {compress_rle, compress_null};
    DecompressFunc decompressors[] = {decompress_rle, decompress_null};
    const char* names[] = {"RLE", "Null"};
    
    uint8_t test_data1[] = {1, 1, 1, 2, 2, 3, 3, 3, 3, 4};
    uint8_t test_data2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    Buffer buffers[] = {
        {test_data1, sizeof(test_data1)},
        {test_data