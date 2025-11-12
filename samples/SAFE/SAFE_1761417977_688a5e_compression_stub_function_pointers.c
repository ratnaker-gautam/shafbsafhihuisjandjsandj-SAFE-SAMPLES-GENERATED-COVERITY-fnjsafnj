//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(Buffer);
typedef Buffer (*DecompressionFunc)(Buffer);

Buffer dummy_compress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size * 2;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0; i < input.size; i++) {
        output.data[i * 2] = input.data[i];
        output.data[i * 2 + 1] = input.data[i] ^ 0xFF;
    }
    
    return output;
}

Buffer dummy_decompress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL || input.size % 2 != 0) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size / 2;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0; i < output.size; i++) {
        if (input.data[i * 2 + 1] != (input.data[i * 2] ^ 0xFF)) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        output.data[i] = input.data[i * 2];
    }
    
    return output;
}

int get_user_choice(void) {
    char input[16];
    int choice;
    
    printf("Choose operation:\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("3. Exit\n");
    printf("Enter choice (1-3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 3) {
        return -1;
    }
    
    return choice;
}

Buffer get_input_data(void) {
    char input[MAX_INPUT_SIZE + 1];
    Buffer buffer;
    
    printf("Enter data to process (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        buffer.data = NULL;
        buffer.size = 0;
        return buffer;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        buffer.data = NULL;
        buffer.size = 0;
        return buffer;
    }
    
    buffer.size = len;
    buffer.data = malloc(len);
    if (buffer.data == NULL) {
        buffer.size = 0;
        return buffer;
    }
    
    memcpy(buffer.data, input, len);
    return buffer;
}

void display_result(Buffer result, const char *operation) {
    if (result.data == NULL || result.size == 0) {
        printf("%s failed or produced no output\n", operation);
        return;
    }
    
    printf("%s result (%zu bytes): ", operation, result.size);
    for (size_t i = 0; i < result.size && i < 50; i++) {
        if (isprint(result.data[i])) {
            printf("%c", result.data[i]);
        } else {
            printf("\\x%02X", result.data[i]);
        }
    }
    if (result.size > 50) {
        printf("...");
    }
    printf("\n");
}

int main(void) {
    CompressionFunc compress_func = dummy_compress;
    DecompressionFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Stub Demo\n");
    printf("============================\n");
    
    while (1) {
        int choice = get_user_choice();
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (choice == 3) {
            printf("Exiting program.\n");
            break;
        }
        
        Buffer input = get_input_data();
        if (input.data == NULL || input.size == 0) {
            printf("No valid input data provided.\n");
            continue;
        }
        
        Buffer result;
        const char *operation;
        
        if (choice == 1) {
            result = compress_func(input);
            operation = "Compression";
        } else {
            result = decompress_func(input);
            operation = "Decompression";
        }
        
        display_result(result, operation);
        
        free(input.data);
        if (result.data != NULL) {
            free(result.data);
        }
        
        printf("\n");
    }
    
    return 0;
}