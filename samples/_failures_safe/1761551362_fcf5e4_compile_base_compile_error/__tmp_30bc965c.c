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
    if (!filename || !buffer) return -1;
    
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;
    
    buffer->size = 0;
    size_t total_read = 0;
    
    while (!feof(file) && total_read < BUFFER_SIZE) {
        size_t bytes_read = fread(buffer->data + total_read, 1, BUFFER_SIZE - total_read, file);
        if (bytes_read == 0) break;
        total_read += bytes_read;
    }
    
    fclose(file);
    buffer->size = total_read;
    return 0;
}

int write_file(const char* filename, const Buffer* buffer) {
    if (!filename || !buffer) return -1;
    
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    
    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    fclose(file);
    
    return (written == buffer->size) ? 0 : -1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (!input || !output || input->size == 0) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (output->size + 3 <= BUFFER_SIZE) {
                output->data[output->size++] = 0xFF;
                output->data[output->size++] = current;
                output->data[output->size++] = (uint8_t)count;
            }
            i += count;
        } else {
            for (size_t j = 0; j < count && output->size < BUFFER_SIZE; j++) {
                output->data[output->size++] = input->data[i + j];
            }
            i += count;
        }
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (!input || !output) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size && output->size < BUFFER_SIZE) {
        if (i + 2 < input->size && input->data[i] == 0xFF) {
            uint8_t value = input->data[i + 1];
            uint8_t count = input->data[i + 2];
            
            if (output->size + count <= BUFFER_SIZE) {
                for (uint8_t j = 0; j < count; j++) {
                    output->data[output->size++] = value;
                }
            }
            i += 3;
        } else {
            if (output->size < BUFFER_SIZE) {
                output->data[output->size++] = input->data[i++];
            }
        }
    }
}

int main() {
    char input_filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    char operation[16];
    
    printf("Enter input filename: ");
    if (!fgets(input_filename, sizeof(input_filename), stdin)) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }
    
    size_t len = strlen(input_filename);
    if (len > 0 && input_filename[len - 1] == '\n') {
        input_filename[len - 1] = '\0';
    }
    
    printf("Enter operation (compress/decompress): ");
    if (!fgets(operation, sizeof(operation), stdin)) {
        fprintf(stderr, "Error reading operation\n");
        return 1;
    }
    
    len = strlen(operation);
    if (len > 0 && operation[len - 1] == '\n') {
        operation[len - 1] = '\0';
    }
    
    printf("Enter output filename: ");
    if (!fgets(output_filename, sizeof(output_filename), stdin)) {
        fprintf(stderr, "Error reading output filename\n");
        return 1;
    }
    
    len = strlen(output_filename);
    if (len > 0 && output_filename[len - 1] == '\n') {
        output_filename[len - 1] = '\0';
    }
    
    if (strlen(input_filename) == 0 || strlen(output_filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    Buffer input, output;
    
    if (read_file(input_filename, &input) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    
    if (strcmp(operation, "compress") == 0) {
        simple_compress(&input, &output);
    } else if (strcmp(operation, "decompress") == 0) {
        simple_decompress(&input, &output);
    } else {
        fprintf(stderr, "Invalid operation.\