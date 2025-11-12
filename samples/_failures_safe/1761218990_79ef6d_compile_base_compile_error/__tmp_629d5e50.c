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
    
    while (in_pos < input.size && out_pos + 2 < MAX_OUTPUT_SIZE) {
        uint8_t current = input.data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input.size && 
               input.data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer compress_null(Buffer input) {
    Buffer output;
    output.data = malloc(input.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    if (input.size > 0 && input.data != NULL) {
        memcpy(output.data, input.data, input.size);
    }
    output.size = input.size;
    return output;
}

Buffer decompress_rle(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
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
        uint8_t count = input.data[in_pos++];
        uint8_t value = input.data[in_pos++];
        
        if (out_pos + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

void process_compression(CompressionFunc compress_func, CompressionFunc decompress_func) {
    char input_str[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input_buffer;
    input_buffer.data = (uint8_t*)input_str;
    input_buffer.size = input_len;
    
    Buffer compressed = compress_func(input_buffer);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Original size: %zu bytes\n", input_len);
    
    if (decompress_func != NULL) {
        Buffer decompressed = decompress_func(compressed);
        if (decompressed.data != NULL && decompressed.size > 0) {
            printf("Decompressed: ");
            fwrite(decompressed.data, 1, decompressed.size, stdout);
            printf("\n");
            free(decompressed.data);
        }
    }
    
    free(compressed.data);
}

int main() {
    CompressionFunc compress_funcs[] = {compress_rle, compress_null};
    CompressionFunc decompress_funcs[] = {decompress_rle, NULL};
    const char *names[] = {"Run-Length Encoding", "Null (no compression)"};
    
    printf("Available compression methods:\n");
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        printf("%zu. %s\n", i + 1, names[i]);
    }
    
    printf("Select method (1-%zu): ", sizeof(names) / sizeof(names[0]));
    
    char choice_str[10];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Invalid selection\n");
        return 1;
    }
    
    long choice = strtol(choice_str, NULL, 10);
    if (choice < 1 || choice > (long)(sizeof(names) / sizeof(names[0]))) {
        printf("Invalid selection\n");
        return 1;
    }
    
    size_t index = (size_t)(choice - 1);
    process_compression(compress_funcs[index], decompress_funcs[index]);
    
    return