//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(const Buffer*);

Buffer null_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    size_t safe_size = input->size * COMPRESSED_MULTIPLIER;
    if (safe_size < input->size) return output;
    
    output.data = malloc(safe_size);
    if (output.data == NULL) return output;
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i] = input->data[i] ^ 0x55;
    }
    output.size = input->size;
    
    return output;
}

Buffer null_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    output.data = malloc(input->size);
    if (output.data == NULL) return output;
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i] = input->data[i] ^ 0x55;
    }
    output.size = input->size;
    
    return output;
}

void process_compression(CompressionFunc func, const char *name) {
    printf("Enter data to %s (max %d bytes): ", name, MAX_INPUT_SIZE);
    
    char input_data[MAX_INPUT_SIZE + 1];
    if (fgets(input_data, sizeof(input_data), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input_data);
    if (len > 0 && input_data[len - 1] == '\n') {
        input_data[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return;
    }
    
    Buffer input = {(uint8_t*)input_data, len};
    Buffer output = func(&input);
    
    if (output.data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    printf("Result (%zu bytes): ", output.size);
    for (size_t i = 0; i < output.size; i++) {
        printf("%02X ", output.data[i]);
    }
    printf("\n");
    
    free(output.data);
}

int main() {
    CompressionFunc compressors[] = {null_compress, null_decompress};
    const char *names[] = {"compress", "decompress"};
    
    printf("Compression Stub Demo\n");
    printf("1. Compress data\n");
    printf("2. Decompress data\n");
    printf("3. Exit\n");
    
    while (1) {
        printf("\nChoose operation: ");
        
        char choice_str[16];
        if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
            break;
        }
        
        int choice = atoi(choice_str);
        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Please enter 1-3.\n");
            continue;
        }
        
        if (choice == 3) {
            break;
        }
        
        process_compression(compressors[choice - 1], names[choice - 1]);
    }
    
    printf("Goodbye!\n");
    return 0;
}