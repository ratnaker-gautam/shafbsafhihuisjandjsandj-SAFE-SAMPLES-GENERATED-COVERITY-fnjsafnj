//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096
#define MAX_FILENAME 256

typedef struct {
    uint8_t data[BUFFER_SIZE];
    size_t size;
} Buffer;

int read_file(const char* filename, Buffer* buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return -1;
    
    buffer->size = fread(buffer->data, 1, BUFFER_SIZE, file);
    int result = ferror(file) ? -1 : 0;
    fclose(file);
    return result;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (filename == NULL || buffer == NULL) return -1;
    if (buffer->size > BUFFER_SIZE) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    int result = (written == buffer->size && !ferror(file)) ? 0 : -1;
    fclose(file);
    return result;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    output->size = 0;
    if (input->size == 0) return;
    
    for (size_t i = 0; i < input->size && output->size < BUFFER_SIZE - 2; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255) {
            count++;
        }
        
        if (output->size + 2 <= BUFFER_SIZE) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
        }
        
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    output->size = 0;
    if (input->size == 0 || input->size % 2 != 0) return;
    
    for (size_t i = 0; i < input->size && output->size < BUFFER_SIZE; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
            output->data[output->size++] = value;
        }
    }
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    int choice;
    
    printf("Simple Run-Length Compression Tool\n");
    printf("1. Compress file\n");
    printf("2. Decompress file\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter input filename: ");
    if (scanf("%255s", input_filename) != 1) {
        printf("Invalid filename\n");
        return 1;
    }
    
    printf("Enter output filename: ");
    if (scanf("%255s", output_filename) != 1) {
        printf("Invalid filename\n");
        return 1;
    }
    
    Buffer input, output;
    
    if (read_file(input_filename, &input) != 0) {
        printf("Error reading input file\n");
        return 1;
    }
    
    if (choice == 1) {
        simple_compress(&input, &output);
        printf("Compressed %zu bytes to %zu bytes\n", input.size, output.size);
    } else {
        simple_decompress(&input, &output);
        printf("Decompressed %zu bytes to %zu bytes\n", input.size, output.size);
    }
    
    if (write_file(output_filename, &output) != 0) {
        printf("Error writing output file\n");
        return 1;
    }
    
    printf("Operation completed successfully\n");
    return 0;
}