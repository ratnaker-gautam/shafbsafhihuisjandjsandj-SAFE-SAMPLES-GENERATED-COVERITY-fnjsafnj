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

typedef Buffer (*CompressionFunc)(Buffer);

Buffer compress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input.size && 
               input.data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 1 || current >= 128) {
            if (out_pos + 3 > MAX_OUTPUT_SIZE) break;
            output.data[out_pos++] = 0xFF;
            output.data[out_pos++] = current;
            output.data[out_pos++] = (uint8_t)count;
        } else {
            if (out_pos + 1 > MAX_OUTPUT_SIZE) break;
            output.data[out_pos++] = current;
        }
        
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer decompress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer output;
    output.data = malloc(MAX_OUTPUT_SIZE);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input.size && out_pos < MAX_OUTPUT_SIZE) {
        if (input.data[in_pos] == 0xFF && in_pos + 2 < input.size) {
            uint8_t value = input.data[in_pos + 1];
            uint8_t count = input.data[in_pos + 2];
            
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            for (uint8_t i = 0; i < count && out_pos < MAX_OUTPUT_SIZE; i++) {
                output.data[out_pos++] = value;
            }
            
            in_pos += 3;
        } else {
            output.data[out_pos++] = input.data[in_pos++];
        }
    }
    
    output.size = out_pos;
    return output;
}

Buffer process_data(Buffer input, CompressionFunc func) {
    if (func == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    return func(input);
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer input;
    input.data = (uint8_t*)malloc(input_len);
    if (input.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    memcpy(input.data, input_str, input_len);
    input.size = input_len;
    
    CompressionFunc compressors[] = {compress_rle, NULL};
    CompressionFunc decompressors[] = {decompress_rle, NULL};
    const char* names[] = {"RLE", NULL};
    
    printf("Available compression methods:\n");
    for (int i = 0; compressors[i] != NULL; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        cleanup_buffer(&input);
        return 1;
    }
    
    if (choice < 1 || compressors[choice - 1] == NULL) {
        fprintf(stderr, "Invalid choice\n");
        cleanup_buffer(&input);
        return 1;
    }
    
    Buffer compressed = process_data(input, compressors[choice - 1]);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        cleanup_buffer(&input);
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size,