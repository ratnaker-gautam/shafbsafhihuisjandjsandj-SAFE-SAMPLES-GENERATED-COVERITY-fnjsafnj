//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output_idx + 2 > output_size) {
            return 0;
        }
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size % 2 != 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
    }
    
    return output_idx;
}

int process_compression(Buffer* input, Buffer* output) {
    if (!validate_input(input->data, input->size)) {
        return 0;
    }
    
    size_t compressed_size = compress_rle(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return 0;
    }
    
    output->size = compressed_size;
    return 1;
}

int process_decompression(Buffer* input, Buffer* output) {
    if (!validate_input(input->data, input->size)) {
        return 0;
    }
    
    size_t decompressed_size = decompress_rle(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) {
        return 0;
    }
    
    output->size = decompressed_size;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buffer->size > 0) {
        printf("\n");
    }
}

int get_user_input(Buffer* buffer) {
    printf("Enter data as hexadecimal bytes (e.g., 41 42 43): ");
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    
    char* token = strtok(line, " ");
    size_t count = 0;
    
    while (token != NULL && count < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            return 0;
        }
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            return 0;
        }
        
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) {
            return 0;
        }
        
        buffer->data[count++] = (uint8_t)byte;
        token = strtok(NULL, " ");
    }
    
    buffer->size = count;
    return count > 0;
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer output_buffer = {0};
    
    printf("RLE Compression/Decompression Tool\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("Choose operation: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (!