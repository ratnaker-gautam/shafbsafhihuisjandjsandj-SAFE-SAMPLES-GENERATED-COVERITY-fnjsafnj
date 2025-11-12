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

int get_user_choice(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    
    return choice;
}

int read_input_data(Buffer* buffer) {
    char input[MAX_INPUT_SIZE * 3];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len % 2 != 0 || len == 0) {
        return 0;
    }
    
    buffer->size = 0;
    for (size_t i = 0; i < len && buffer->size < MAX_INPUT_SIZE; i += 2) {
        char hex[3] = {input[i], input[i + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (endptr == hex || *endptr != '\0' || value < 0 || value > 255) {
            return 0;
        }
        buffer->data[buffer->size++] = (uint8_t)value;
    }
    
    return 1;
}

int main(void) {
    Buffer input_data = {0};
    Buffer output_data = {0};
    
    printf("Compression Tool\n");
    printf("1. Compress data\n");
    printf("2. Decompress data\n");
    printf("3. Exit\n");
    printf("Choose option: ");
    
    int choice = get_user_choice();
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;