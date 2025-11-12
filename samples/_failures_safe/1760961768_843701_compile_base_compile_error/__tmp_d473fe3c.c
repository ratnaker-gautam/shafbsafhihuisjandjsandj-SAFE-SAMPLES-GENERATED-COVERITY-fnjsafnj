//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

Buffer run_length_encode(Buffer input) {
    if (input.data == NULL || input.size == 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input.size && out_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input.data[i];
        uint8_t count = 1;
        
        while (i + count < input.size && count < 255 && 
               input.data[i + count] == current) {
            count++;
        }
        
        output[out_index++] = count;
        output[out_index++] = current;
        i += count;
    }
    
    Buffer result;
    result.data = output;
    result.size = out_index;
    return result;
}

Buffer run_length_decode(Buffer input) {
    if (input.data == NULL || input.size == 0 || input.size % 2 != 0) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < input.size && out_index < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t count = input.data[i];
        uint8_t value = input.data[i + 1];
        
        for (uint8_t j = 0; j < count && out_index < MAX_OUTPUT_SIZE; j++) {
            output[out_index++] = value;
        }
    }
    
    Buffer result;
    result.data = output;
    result.size = out_index;
    return result;
}

void print_buffer_hex(Buffer buf) {
    if (buf.data == NULL || buf.size == 0) {
        printf("Empty buffer\n");
        return;
    }
    
    for (size_t i = 0; i < buf.size; i++) {
        printf("%02X ", buf.data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf.size % 16 != 0) {
        printf("\n");
    }
}

int get_user_choice(void) {
    char input[16];
    printf("Choose operation:\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("3. Exit\n");
    printf("Enter choice (1-3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (choice < 1 || choice > 3) {
        return -1;
    }
    
    return (int)choice;
}

Buffer get_input_data(void) {
    char input[MAX_INPUT_SIZE];
    Buffer result = {NULL, 0};
    
    printf("Enter data (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return result;
    }
    
    uint8_t *data = malloc(MAX_INPUT_SIZE);
    if (data == NULL) {
        return result;
    }
    
    size_t data_index = 0;
    char *token = strtok(input, " \t\n");
    
    while (token != NULL && data_index < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            free(data);
            result.data = NULL;
            result.size = 0;
            return result;
        }
        
        if (!isxdigit(token[0]) || !isxdigit(token[1])) {
            free(data);
            result.data = NULL;
            result.size = 0;
            return result;
        }
        
        char *endptr;
        long value = strtol(token, &endptr, 16);
        if (endptr != token + 2 || value < 0 || value > 255) {
            free(data);
            result.data = NULL;
            result.size = 0;
            return result;
        }
        
        data[data_index++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    result.data = data;
    result.size = data_index;
    return result;
}

int main(void) {
    CompressionFunc compress = run_length_encode;
    DecompressionFunc decompress = run_length_decode;
    
    printf("Simple Run-Length Compression Demo\n");
    printf("==================================\n");
    
    while (1) {
        int choice = get_user_choice();
        if (choice == -1) {
            printf("Invalid choice. Please try again