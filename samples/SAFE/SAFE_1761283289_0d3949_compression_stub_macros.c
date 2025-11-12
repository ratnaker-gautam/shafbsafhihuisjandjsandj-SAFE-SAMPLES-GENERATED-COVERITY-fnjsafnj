//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
} file_header_t;

static size_t safe_read_input(char *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0 || max_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t total_read = 0;
    int c;
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total_read++] = (char)c;
    }
    buffer[total_read] = '\0';
    return total_read;
}

static void naive_compress(const char *input, size_t input_len, char *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len == 0) {
        *output_len = 0;
        return;
    }
    
    file_header_t header;
    header.magic = MAGIC_HEADER;
    header.original_size = (uint32_t)input_len;
    
    size_t compressed_data_len = 0;
    for (size_t i = 0; i < input_len && compressed_data_len < MAX_OUTPUT_SIZE - sizeof(file_header_t) - 1; i += CHUNK_SIZE) {
        char compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE && i + j < input_len; j++) {
            compressed_byte ^= input[i + j];
        }
        output[sizeof(file_header_t) + compressed_data_len++] = compressed_byte;
    }
    
    header.compressed_size = (uint32_t)compressed_data_len;
    memcpy(output, &header, sizeof(file_header_t));
    *output_len = sizeof(file_header_t) + compressed_data_len;
}

static void write_output(const char *data, size_t data_len) {
    if (data == NULL || data_len == 0 || data_len > MAX_OUTPUT_SIZE) {
        return;
    }
    
    for (size_t i = 0; i < data_len; i++) {
        if (putchar(data[i]) == EOF) {
            break;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    char output_buffer[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    fflush(stdout);
    
    size_t input_len = safe_read_input(input_buffer, sizeof(input_buffer));
    if (input_len == 0) {
        fprintf(stderr, "Error: No input received or input too large\n");
        return EXIT_FAILURE;
    }
    
    size_t output_len;
    naive_compress(input_buffer, input_len, output_buffer, &output_len);
    
    if (output_len == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    write_output(output_buffer, output_len);
    
    printf("\nCompression complete: %zu -> %zu bytes\n", input_len, output_len);
    
    return EXIT_SUCCESS;
}