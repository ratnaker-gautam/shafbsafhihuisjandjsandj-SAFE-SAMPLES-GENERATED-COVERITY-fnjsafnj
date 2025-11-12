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
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
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
    
    uint8_t* resized = realloc(output_data, output_index);
    if (resized != NULL) {
        output_data = resized;
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
        if (input->data[i] == 0) {
            Buffer empty = {NULL, 0};
            return empty;
        }
        if (SIZE_MAX - output_size < input->data[i]) {
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
    
    size_t output_index = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (output_index >= output_size) {
                free(output_data);
                Buffer empty = {NULL, 0};
                return empty;
            }
            output_data[output_index++] = value;
        }
    }
    
    Buffer result = {output_data, output_size};
    return result;
}

Buffer compress_copy(const Buffer* input) {
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

Buffer decompress_copy(const Buffer* input) {
    return compress_copy(input);
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const char* name) {
    printf("Testing %s compression\n", name);
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
    
    Buffer input_buffer = {(uint8_t*)input_text, len};
    
    Buffer compressed = compress(&input_buffer);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Original size: %zu bytes\n", len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / len) * 100);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == len && memcmp(decompressed.data, input_text, len)