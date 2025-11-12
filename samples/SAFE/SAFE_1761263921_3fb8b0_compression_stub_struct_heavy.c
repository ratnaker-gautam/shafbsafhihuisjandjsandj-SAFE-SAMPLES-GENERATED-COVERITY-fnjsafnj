//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
    uint16_t algorithm;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) {
        return 0;
    }
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) {
        return 0;
    }
    
    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }
    
    memcpy(new_block->data, data, size);
    new_block->size = size;
    new_block->next = NULL;
    
    if (ctx->first_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }
    
    ctx->total_blocks++;
    ctx->header.compressed_size += size;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->first_block == NULL) {
        return;
    }
    
    uint8_t checksum = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current != NULL) {
        for (size_t i = 0; i < current->size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    
    ctx->header.checksum = checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) {
        return 0;
    }
    
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t block_size = (input_size - processed > 1024) ? 1024 : (input_size - processed);
        
        if (!add_block(ctx, input + processed, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    calculate_checksum(ctx);
    return 1;
}

void free_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int write_compressed_file(struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || strlen(filename) == 0) {
        return 0;
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        if (fwrite(current->data, 1, current->size, file) != current->size) {
            fclose(file);
            return 0;
        }
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[2048];
    for (int i = 0; i < 2048; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    if (!compress_data(&ctx, test_data, sizeof(test_data))) {
        printf("Compression failed\n");
        free_compression_context(&ctx);
        return 1;
    }
    
    if (!write_compressed_file(&ctx, "test_compressed.bin")) {
        printf("File write failed\n");
        free_compression_context(&ctx);
        return 1;
    }
    
    printf("Compression completed: %u bytes -> %u bytes\n", 
           ctx.header.original_size, ctx.header.compressed_size);
    printf("Checksum: 0x%02X\n", ctx.header.checksum);
    
    free_compression_context(&ctx);
    return 0;
}