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
    
    size_t output_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
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
        
        if (out_index + count > output_size) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output_data[out_index++] = value;
        }
    }
    
    Buffer result = {output_data, output_size};
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
    CompressFunc compress = compress_rle;
    DecompressFunc decompress = decompress_rle;
    
    const char* test_string = "AAAABBBCCDAA";
    size_t input_len = strlen(test_string);
    
    Buffer input = {(uint8_t*)test_string, input_len};
    
    printf("Original: %s\n", test_string);
    printf("Original size: %zu bytes\n", input_len);
    
    Buffer compressed = compress(&input);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free_buffer(&compressed);
        return 1;
    }
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input.data, input_len) == 0) {
        printf("Decompression successful: %.*s\n", (int)decompressed.size, decompressed.data);
    } else {
        printf("Decompression failed - data mismatch\n");
    }
    
    free_buffer(&compressed);
    free_buffer(&decompressed);
    
    return 0;
}