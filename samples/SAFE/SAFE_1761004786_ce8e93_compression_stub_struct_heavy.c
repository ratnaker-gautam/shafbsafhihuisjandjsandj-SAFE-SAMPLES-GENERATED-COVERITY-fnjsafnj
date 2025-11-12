//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    size_t capacity;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
    int compression_level;
};

struct CompressionResult {
    int success;
    size_t original_size;
    size_t compressed_size;
    double ratio;
};

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (level < 0 || level > 9) level = 6;
    memset(ctx, 0, sizeof(*ctx));
    ctx->compression_level = level;
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
}

int add_data_to_context(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    if (size > 1024 * 1024) return 0;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (!block) return 0;
    
    block->data = malloc(size);
    if (!block->data) {
        free(block);
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->size = size;
    block->capacity = size;
    block->next = NULL;
    
    if (!ctx->first_block) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.original_size += size;
    return 1;
}

struct CompressionResult perform_compression(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    if (!ctx || !ctx->first_block) return result;
    
    size_t compressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current) {
        size_t block_compressed = current->size;
        if (ctx->compression_level > 6) {
            block_compressed = current->size * 85 / 100;
        } else if (ctx->compression_level > 3) {
            block_compressed = current->size * 90 / 100;
        } else {
            block_compressed = current->size * 95 / 100;
        }
        
        if (block_compressed < 1) block_compressed = 1;
        compressed += block_compressed;
        current = current->next;
    }
    
    ctx->header.compressed_size = compressed;
    result.success = 1;
    result.original_size = ctx->header.original_size;
    result.compressed_size = compressed;
    
    if (result.original_size > 0) {
        result.ratio = (double)result.compressed_size / result.original_size;
    } else {
        result.ratio = 0.0;
    }
    
    return result;
}

void free_compression_context(struct CompressionContext *ctx) {
    if (!ctx) return;
    
    struct CompressionBlock *current = ctx->first_block;
    while (current) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 7);
    
    uint8_t test_data1[] = "This is some test data for compression.";
    uint8_t test_data2[] = "More test data to compress.";
    uint8_t test_data3[] = "Final chunk of test data.";
    
    if (!add_data_to_context(&ctx, test_data1, sizeof(test_data1) - 1)) {
        printf("Failed to add data 1\n");
        return 1;
    }
    
    if (!add_data_to_context(&ctx, test_data2, sizeof(test_data2) - 1)) {
        printf("Failed to add data 2\n");
        free_compression_context(&ctx);
        return 1;
    }
    
    if (!add_data_to_context(&ctx, test_data3, sizeof(test_data3) - 1)) {
        printf("Failed to add data 3\n");
        free_compression_context(&ctx);
        return 1;
    }
    
    struct CompressionResult result = perform_compression(&ctx);
    
    if (result.success) {
        printf("Compression completed successfully\n");
        printf("Original size: %zu bytes\n", result.original_size);
        printf("Compressed size: %zu bytes\n", result.compressed_size);
        printf("Compression ratio: %.2f\n", result.ratio);
        printf("Magic: 0x%08X\n", ctx.header.magic);
        printf("Algorithm: %u\n", ctx.header.algorithm);
    } else {
        printf("Compression failed\n");
    }
    
    free_compression_context(&ctx);
    return 0;
}