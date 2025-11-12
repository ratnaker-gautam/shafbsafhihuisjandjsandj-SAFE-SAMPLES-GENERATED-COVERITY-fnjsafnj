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
        if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':' || 
            filename[i] == '*' || filename[i] == '?' || filename[i] == '"' || 
            filename[i] == '<' || filename[i] == '>' || filename[i] == '|') {
            return 0;
        }
    }
    return 1;
}

int compress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }

    FILE* input = fopen(input_filename, "rb");
    if (input == NULL) {
        return -1;
    }

    FILE* output = fopen(output_filename, "wb");
    if (output == NULL) {
        fclose(input);
        return -1;
    }

    FileHeader header;
    strncpy(header.filename, input_filename, MAX_FILENAME - 1);
    header.filename[MAX_FILENAME - 1] = '\0';

    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    if (file_size < 0 || file_size > SIZE_MAX) {
        fclose(input);
        fclose(output);
        return -1;
    }

    header.original_size = (size_t)file_size;
    header.compressed_size = header.original_size;

    if (fwrite(&header, sizeof(FileHeader), 1, output) != 1) {
        fclose(input);
        fclose(output);
        return -1;
    }

    uint8_t buffer[BUFFER_SIZE];
    size_t total_written = 0;

    while (!feof(input)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, input);
        if (ferror(input)) {
            fclose(input);
            fclose(output);
            return -1;
        }

        if (bytes_read > 0) {
            size_t bytes_written = fwrite(buffer, 1, bytes_read, output);
            if (bytes_written != bytes_read || ferror(output)) {
                fclose(input);
                fclose(output);
                return -1;
            }
            total_written += bytes_written;
        }
    }

    fclose(input);
    fclose(output);

    if (total_written != header.original_size) {
        remove(output_filename);
        return -1;
    }

    return 0;
}

int decompress_file(const char* input_filename, const char* output_filename) {
    if (!validate_filename(input_filename) || !validate_filename(output_filename)) {
        return -1;
    }

    FILE* input = fopen(input_filename, "rb");
    if (input == NULL) {
        return -1;
    }

    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, input) != 1) {
        fclose(input);
        return -1;
    }

    if (!validate_filename(header.filename)) {
        fclose(input);
        return -1;
    }

    FILE* output = fopen(output_filename, "wb");
    if (output == NULL) {
        fclose(input);
        return -1;
    }

    uint8_t buffer[BUFFER_SIZE];
    size_t total_read = 0;

    while (!feof(input)) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, input);
        if (ferror(input)) {
            fclose(input);
            fclose(output);
            return -1;
        }

        if (bytes_read > 0) {
            size_t bytes_written = fwrite(buffer, 1, bytes_read, output);
            if (bytes_written != bytes_read || ferror(output)) {
                fclose(input);
                fclose(output);
                return -1;
            }
            total_read += bytes_read;
        }
    }

    fclose(input);
    fclose(output);

    if (total_read != header.compressed_size) {
        remove(output_filename);
        return -1;
    }

    return 0;
}

int main() {
    printf("Simple File Compression Tool\n");
    printf("1. Compress file\n");
    printf("2. Decompress file\n");
    printf("3. Exit\n");

    int choice;
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];

    switch (choice) {
        case 1:
            printf("Enter input filename: ");
            if (scanf("%255s", input_file) != 1) {
                printf("Invalid filename\n");
                return 1;
            }
            printf("Enter output filename: ");
            if (scanf("%255s", output_file) != 1) {
                printf("Invalid filename\n");
                return 1;
            }