//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 4096
#define MAX_FILENAME 256

typedef struct {
    char filename[MAX_FILENAME];
    size_t original_size;
    size_t compressed_size;
} FileHeader;

int validate_filename(const char* filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':') {
            return 0;
        }
    }
    return 1;
}

int compress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }

    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return -1;
    }

    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }

    FileHeader header;
    strncpy(header.filename, input_filename, MAX_FILENAME - 1);
    header.filename[MAX_FILENAME - 1] = '\0';
    header.original_size = 0;
    header.compressed_size = 0;

    uint8_t input_buffer[BUFFER_SIZE];
    uint8_t output_buffer[BUFFER_SIZE];
    size_t bytes_read;
    size_t total_input = 0;
    size_t total_output = 0;

    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        total_input += bytes_read;
        
        size_t output_pos = 0;
        for (size_t i = 0; i < bytes_read; i++) {
            if (output_pos >= BUFFER_SIZE - 2) {
                break;
            }
            
            uint8_t current = input_buffer[i];
            uint8_t count = 1;
            
            while (i + 1 < bytes_read && input_buffer[i + 1] == current && count < 255) {
                count++;
                i++;
            }
            
            output_buffer[output_pos++] = count;
            output_buffer[output_pos++] = current;
        }
        
        if (fwrite(output_buffer, 1, output_pos, output_file) != output_pos) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        total_output += output_pos;
    }

    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    header.original_size = total_input;
    header.compressed_size = total_output;

    fseek(output_file, 0, SEEK_SET);
    if (fwrite(&header, sizeof(FileHeader), 1, output_file) != 1) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int decompress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }

    FILE* input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return -1;
    }

    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, input_file) != 1) {
        fclose(input_file);
        return -1;
    }

    FILE* output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }

    uint8_t input_buffer[BUFFER_SIZE];
    size_t bytes_read;
    size_t total_written = 0;

    while ((bytes_read = fread(input_buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        for (size_t i = 0; i < bytes_read; i += 2) {
            if (i + 1 >= bytes_read) {
                break;
            }
            
            uint8_t count = input_buffer[i];
            uint8_t value = input_buffer[i + 1];
            
            for (uint8_t j = 0; j < count; j++) {
                if (fwrite(&value, 1, 1, output_file) != 1) {
                    fclose(input_file);
                    fclose(output_file);
                    return -1;
                }
                total_written++;
            }
        }
    }

    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int main() {
    printf("Simple File Compression Tool\n");
    printf("1. Compress file\n");
    printf("2. Decompress file\n");
    printf("Choose option: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    
    printf("Enter input filename: ");
    if (scanf("%255s", input_file) != 1) {