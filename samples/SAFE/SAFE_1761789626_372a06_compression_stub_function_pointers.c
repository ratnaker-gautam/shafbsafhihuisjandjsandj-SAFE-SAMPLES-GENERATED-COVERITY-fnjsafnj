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

Buffer dummy_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    size_t max_output = input->size * COMPRESSED_MULTIPLIER;
    if (max_output < input->size) {
        return output;
    }
    
    output.data = malloc(max_output);
    if (output.data == NULL) {
        return output;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i] = input->data[i] + 1;
    }
    output.size = input->size;
    
    return output;
}

Buffer dummy_decompress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    output.data = malloc(input->size);
    if (output.data == NULL) {
        return output;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i] = input->data[i] - 1;
    }
    output.size = input->size;
    
    return output;
}

void process_compression(CompressionFunc func, const char *name) {
    printf("Enter data to %s (max %d characters): ", name, MAX_INPUT_SIZE - 1);
    
    char input_str[MAX_INPUT_SIZE];
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return;
    }
    
    Buffer input = {(uint8_t*)input_str, len};
    Buffer output = func(&input);
    
    if (output.data == NULL) {
        printf("Error: %s failed\n", name);
        return;
    }
    
    printf("Result (%zu bytes): ", output.size);
    for (size_t i = 0; i < output.size; i++) {
        printf("%c", (char)output.data[i]);
    }
    printf("\n");
    
    free(output.data);
}

int main() {
    CompressionFunc functions[] = {dummy_compress, dummy_decompress};
    const char *names[] = {"compress", "decompress"};
    const int num_funcs = 2;
    
    while (1) {
        printf("\nCompression Stub Menu:\n");
        printf("1. Compress data\n");
        printf("2. Decompress data\n");
        printf("3. Exit\n");
        printf("Choose option: ");
        
        char choice_str[10];
        if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
            break;
        }
        
        int choice = atoi(choice_str);
        if (choice < 1 || choice > 3) {
            printf("Invalid choice\n");
            continue;
        }
        
        if (choice == 3) {
            break;
        }
        
        process_compression(functions[choice - 1], names[choice - 1]);
    }
    
    printf("Goodbye!\n");
    return 0;
}