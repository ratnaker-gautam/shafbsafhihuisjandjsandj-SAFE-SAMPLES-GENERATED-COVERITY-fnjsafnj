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
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && count < 255 && 
               input->data[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > max_output) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        output_data[out_idx++] = (uint8_t)count;
        output_data[out_idx++] = current;
        in_idx += count;
    }
    
    Buffer result = {output_data, out_idx};
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
        if (max_output < input->data[i]) {
            Buffer empty = {NULL, 0};
            return empty;
        }
    }
    
    uint8_t* output_data = malloc(max_output);
    if (output_data == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > max_output || out_idx + count < out_idx) {
            free(output_data);
            Buffer empty = {NULL, 0};
            return empty;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output_data[out_idx++] = value;
        }
    }
    
    Buffer result = {output_data, out_idx};
    return result;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    printf("Enter text to compress (max 1000 characters): ");
    char input_text[1001];
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input_buf = {(uint8_t*)input_text, len};
    
    Buffer compressed = compress(&input_buf);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", len, compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / len * 100);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == len && memcmp(decompressed.data, input_text, len) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
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