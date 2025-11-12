//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compress_file(const char *input_filename, const char *output_filename, CompressFunc compress) {
    if (input_filename == NULL || output_filename == NULL || compress == NULL) {
        return -1;
    }
    
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return -1;
    }
    
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }
    
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    
    size_t bytes_read;
    while ((bytes_read = fread(input_buffer, 1, MAX_INPUT_SIZE, input_file)) > 0) {
        size_t compressed_size = compress(input_buffer, bytes_read, output_buffer, MAX_OUTPUT_SIZE);
        if (compressed_size == 0) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        if (fwrite(output_buffer, 1, compressed_size, output_file) != compressed_size) {
            fclose(input_file);
            fclose(output_file);
            return -1;
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

int decompress_file(const char *input_filename, const char *output_filename, DecompressFunc decompress) {
    if (input_filename == NULL || output_filename == NULL || decompress == NULL) {
        return -1;
    }
    
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        return -1;
    }
    
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }
    
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    
    size_t bytes_read;
    while ((bytes_read = fread(input_buffer, 1, MAX_INPUT_SIZE, input_file)) > 0) {
        size_t decompressed_size = decompress(input_buffer, bytes_read, output_buffer, MAX_OUTPUT_SIZE);
        if (decompressed_size == 0) {
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        
        if (fwrite(output_buffer, 1, decompressed_size, output_file) != decompressed_size) {
            fclose(input_file);
            fclose(output_file);
            return -1;
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

void test_sample_data(void) {
    uint8_t test_input[] = "AAAABBBCCDAA";
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    size_t compressed_size = run_length_encode(test_input, sizeof(test_input) - 1, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size > 0) {
        size