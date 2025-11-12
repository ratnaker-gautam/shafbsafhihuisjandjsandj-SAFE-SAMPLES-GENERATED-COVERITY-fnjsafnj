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
    
    size_t compressed_size = input->size * COMPRESSED_MULTIPLIER;
    if (compressed_size < input->size) return output;
    
    output.data = malloc(compressed_size);
    if (output.data == NULL) return output;
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i * 2] = input->data[i];
        output.data[i * 2 + 1] = input->data[i] ^ 0xFF;
    }
    output.size = compressed_size;
    return output;
}

Buffer null_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size % 2 != 0) return output;
    
    size_t decompressed_size = input->size / 2;
    output.data = malloc(decompressed_size);
    if (output.data == NULL) return output;
    
    for (size_t i = 0; i < decompressed_size; i++) {
        if (input->data[i * 2 + 1] != (input->data[i * 2] ^ 0xFF)) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        output.data[i] = input->data[i * 2];
    }
    output.size = decompressed_size;
    return output;
}

void process_compression(CompressionFunc func, const char *name) {
    printf("Enter data to %s (max %d bytes): ", name, MAX_INPUT_SIZE);
    
    char input_str[MAX_INPUT_SIZE + 1];
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    Buffer input = {(uint8_t*)input_str, input_len};
    Buffer output = func(&input);
    
    if (output.data == NULL) {
        printf("%s failed\n", name);
        return;
    }
    
    printf("Result (%zu bytes): ", output.size);
    for (size_t i = 0; i < output.size && i < 64; i++) {
        printf("%02X ", output.data[i]);
    }
    if (output.size > 64) printf("...");
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
        if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) break;
        
        int choice = atoi(choice_str);
        
        if (choice == 3) break;
        
        if (choice == 1 || choice == 2) {
            process_compression(compressors[choice - 1], names[choice - 1]);
        } else {
            printf("Invalid choice\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}