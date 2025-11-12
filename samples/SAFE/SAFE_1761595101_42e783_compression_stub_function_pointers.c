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

typedef Buffer (*CompressFunc)(Buffer);
typedef Buffer (*DecompressFunc)(Buffer);

Buffer dummy_compress(Buffer input) {
    Buffer output;
    if (input.size == 0 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size + 4;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    output.data[0] = 'C';
    output.data[1] = 'M';
    output.data[2] = 'P';
    output.data[3] = 'R';
    memcpy(output.data + 4, input.data, input.size);
    
    return output;
}

Buffer dummy_decompress(Buffer input) {
    Buffer output;
    if (input.size < 4 || input.data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    if (input.data[0] != 'C' || input.data[1] != 'M' || 
        input.data[2] != 'P' || input.data[3] != 'R') {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input.size - 4;
    if (output.size == 0) {
        output.data = NULL;
        return output;
    }
    
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input.data + 4, output.size);
    return output;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    char input_str[MAX_INPUT_SIZE];
    int choice;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return;
    }
    
    printf("Choose operation: 1=Compress, 2=Decompress: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return;
    }
    
    while (getchar() != '\n');
    
    Buffer input_buf;
    input_buf.data = (uint8_t*)input_str;
    input_buf.size = len;
    
    Buffer result;
    
    if (choice == 1) {
        result = compress(input_buf);
        if (result.data != NULL && result.size > 0) {
            printf("Compressed data (%zu bytes): ", result.size);
            for (size_t i = 0; i < result.size; i++) {
                printf("%02X ", result.data[i]);
            }
            printf("\n");
            free(result.data);
        } else {
            printf("Compression failed\n");
        }
    } else if (choice == 2) {
        result = decompress(input_buf);
        if (result.data != NULL && result.size > 0) {
            printf("Decompressed text (%zu bytes): ", result.size);
            fwrite(result.data, 1, result.size, stdout);
            printf("\n");
            free(result.data);
        } else {
            printf("Decompression failed\n");
        }
    } else {
        printf("Invalid operation\n");
    }
}

int main() {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}