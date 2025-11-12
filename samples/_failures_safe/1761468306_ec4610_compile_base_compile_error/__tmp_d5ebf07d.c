//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_len + 3 > output_max) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output[output_len++] = current;
        output[output_len++] = (uint8_t)count;
        
        i = j;
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    for (i = 0; i < input_len; i += 2) {
        if (i + 1 >= input_len) {
            return 0;
        }
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output_len + count > output_max) {
            return 0;
        }
        
        size_t k = 0;
        do {
            output[output_len++] = value;
            k++;
        } while (k < count);
    }
    
    return output_len;
}

void process_compression(Buffer* input, Buffer* output) {
    initialize_buffer(output);
    
    size_t compressed_size = simple_compress(input->data, input->length, output->data, MAX_OUTPUT_SIZE);
    if (compressed_size > 0) {
        output->length = compressed_size;
    }
}

void process_decompression(Buffer* input, Buffer* output) {
    initialize_buffer(output);
    
    if (input->length % 2 != 0) {
        return;
    }
    
    size_t decompressed_size = simple_decompress(input->data, input->length, output->data, MAX_OUTPUT_SIZE);
    if (decompressed_size > 0) {
        output->length = decompressed_size;
    }
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->length == 0) {
        return;
    }
    
    for (size_t idx = 0; idx < buf->length; idx++) {
        printf("%02X", buf->data[idx]);
        if ((idx + 1) % 16 == 0 && idx + 1 < buf->length) {
            printf("\n");
        } else if (idx + 1 < buf->length) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input_data;
    Buffer compressed_data;
    Buffer decompressed_data;
    
    initialize_buffer(&input_data);
    initialize_buffer(&compressed_data);
    initialize_buffer(&decompressed_data);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too large.\n");
        return 1;
    }
    
    memcpy(input_data.data, temp_input, input_len);
    input_data.length = input_len;
    
    printf("\nOriginal data (%zu bytes):\n", input_data.length);
    print_buffer_hex(&input_data);
    
    process_compression(&input_data, &compressed_data);
    printf("\nCompressed data (%zu bytes):\n", compressed_data.length);
    print_buffer_hex(&compressed_data);
    
    process_decompression(&compressed_data, &decompressed_data);
    printf("\nDecompressed data (%zu bytes):\n", decompressed_data.length);
    print_buffer_hex(&decompressed_data);
    
    if (decompressed_data.length == input_data.length && 
        memcmp(decompressed