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
    if (input.data == NULL || input.size == 0 || input.size > MAX_INPUT_SIZE) {
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
        
        if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
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
        
        if (out_pos + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

void process_compression(CompressionFunc func, const char* name) {
    char input_str[MAX_INPUT_SIZE + 1];
    
    printf("Enter data to %s: ", name);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    Buffer input;
    input.data = (uint8_t*)input_str;
    input.size = len;
    
    Buffer result = func(input);
    
    if (result.data == NULL || result.size == 0) {
        printf("Processing failed\n");
        return;
    }
    
    printf("Result (%zu bytes): ", result.size);
    for (size_t i = 0; i < result.size && i < 50; i++) {
        printf("%02X ", result.data[i]);
    }
    if (result.size > 50) printf("...");
    printf("\n");
    
    if (result.data != (uint8_t*)input_str) {
        free(result.data);
    }
}

int main() {
    CompressionFunc compressors[] = {compress_rle, decompress_rle};
    const char* names[] = {"compress", "decompress"};
    const int num_funcs = 2;
    
    printf("Compression Stub Demo\n");
    printf("1. Run-length encode\n");
    printf("2. Run-length decode\n");
    printf("3. Exit\n");
    
    while (1) {
        printf("\nChoose operation: ");
        
        char choice_str[10];
        if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
            break;
        }
        
        int choice = atoi(choice_str);
        
        if (choice == 3) {
            break;
        }
        
        if (choice >= 1 && choice <= 2) {
            process_compression(compressors[choice - 1], names[choice - 1]);
        } else {
            printf("Invalid choice\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}