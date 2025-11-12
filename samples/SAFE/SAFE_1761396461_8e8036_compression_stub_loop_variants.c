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

int read_input(Buffer* input) {
    char temp[MAX_INPUT_SIZE + 2];
    
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_SIZE) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)temp[i]) && temp[i] != ' ') {
            return 0;
        }
    }
    
    memcpy(input->data, temp, len);
    input->length = len;
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    output->length = out_idx;
}

void display_compressed(const Buffer* compressed) {
    for (size_t i = 0; i < compressed->length; i += 2) {
        if (i + 1 < compressed->length) {
            printf("%c%u", compressed->data[i], (unsigned int)compressed->data[i + 1]);
        }
    }
    printf("\n");
}

void run_length_decode(const Buffer* compressed, Buffer* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < compressed->length) {
        if (i + 1 >= compressed->length) {
            break;
        }
        
        uint8_t character = compressed->data[i];
        uint8_t count = compressed->data[i + 1];
        
        if (out_idx + count >= MAX_INPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = character;
        }
        
        i += 2;
    }
    
    output->length = out_idx;
}

void display_original(const Buffer* original) {
    for (size_t i = 0; i < original->length; i++) {
        putchar(original->data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (input.length == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    simple_compress(&input, &compressed);
    
    printf("Compressed: ");
    display_compressed(&compressed);
    
    printf("Compression ratio: %.2f\n", 
           compressed.length ? (float)input.length / compressed.length : 0.0f);
    
    run_length_decode(&compressed, &decompressed);
    
    if (decompressed.length == input.length && 
        memcmp(decompressed.data, input.data, input.length) == 0) {
        printf("Decompression successful: ");
        display_original(&decompressed);
    } else {
        fprintf(stderr, "Error: Decompression failed\n");
        return 1;
    }
    
    return 0;
}