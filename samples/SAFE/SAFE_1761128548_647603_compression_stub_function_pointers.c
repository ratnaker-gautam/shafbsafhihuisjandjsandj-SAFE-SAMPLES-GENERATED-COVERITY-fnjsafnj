//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    
    if (input.data[0] != 'C' || input.data[1] != 'M' || input.data[2] != 'P' || input.data[3] != 'R') {
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

Buffer read_input(void) {
    Buffer buf;
    char temp[MAX_INPUT_SIZE];
    
    printf("Enter data to process: ");
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        buf.data = NULL;
        buf.size = 0;
        return buf;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        buf.data = NULL;
        buf.size = 0;
        return buf;
    }
    
    buf.size = len;
    buf.data = malloc(buf.size);
    if (buf.data == NULL) {
        buf.size = 0;
        return buf;
    }
    
    memcpy(buf.data, temp, buf.size);
    return buf;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer original = read_input();
    if (original.data == NULL || original.size == 0) {
        printf("No valid input provided.\n");
        return;
    }
    
    printf("Original data (%zu bytes): ", original.size);
    fwrite(original.data, 1, original.size, stdout);
    printf("\n");
    
    Buffer compressed = compress(original);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        free(original.data);
        return;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    fwrite(compressed.data, 1, compressed.size, stdout);
    printf("\n");
    
    Buffer decompressed = decompress(compressed);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
    } else {
        printf("Decompressed data (%zu bytes): ", decompressed.size);
        fwrite(decompressed.data, 1, decompressed.size, stdout);
        printf("\n");
        
        if (decompressed.size == original.size && 
            memcmp(decompressed.data, original.data, original.size) == 0) {
            printf("Round-trip successful.\n");
        } else {
            printf("Round-trip failed.\n");
        }
        free(decompressed.data);
    }
    
    free(original.data);
    free(compressed.data);
}

int main(void) {
    CompressFunc compress_func = dummy_compress;
    DecompressFunc decompress_func = dummy_decompress;
    
    printf("Simple Compression Stub Demo\n");
    printf("============================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}