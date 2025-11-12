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

    if (input_len % 2 != 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    for (i = 0; i < input_len; i += 2) {
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
    output->length = simple_compress(input->data, input->length, output->data, MAX_OUTPUT_SIZE);
}

void process_decompression(Buffer* input, Buffer* output) {
    initialize_buffer(output);
    output->length = simple_decompress(input->data, input->length, output->data, MAX_OUTPUT_SIZE);
}

int get_user_input(Buffer* buf) {
    printf("Enter data to process (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    memcpy(buf->data, temp, len);
    buf->length = len;
    return 1;
}

void display_buffer(const Buffer* buf, const char* label) {
    printf("%s (%zu bytes): ", label, buf->length);
    
    size_t i = 0;
    while (i < buf->length) {
        if (isprint(buf->data[i])) {
            putchar(buf->data[i]);
        } else {
            printf("\\x%02X", buf->data[i]);
        }
        i++;
    }
    printf("\n");
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    if (!get_user_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.length == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    display_buffer(&input, "Original");
    
    process_compression(&input, &compressed);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    display_buffer(&compressed, "Compressed");
    
    process_decompression(&compressed, &decompressed);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    display_buffer(&decompressed, "Decompressed");
    
    if (decompressed.length == input.length && 
        memcmp(decompressed.data, input.data, input.length) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
        return 1;
    }
    
    return 0;
}