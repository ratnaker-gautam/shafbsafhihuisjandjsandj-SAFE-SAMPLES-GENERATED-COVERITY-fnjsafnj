//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t compressed_size;
    uint32_t original_size;
    uint8_t algorithm;
    uint8_t flags;
    uint16_t reserved;
};

struct CompressionBlock {
    uint32_t block_offset;
    uint32_t block_size;
    uint32_t compressed_block_size;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return 0;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return 0;
    }
    
    memset(ctx->blocks, 0, max_blocks * sizeof(struct CompressionBlock));
    ctx->max_blocks = max_blocks;
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
    
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->blocks != NULL) {
        for (uint32_t i = 0; i < ctx->block_count; i++) {
            if (ctx->blocks[i].data != NULL) {
                free(ctx->blocks[i].data);
            }
        }
        free(ctx->blocks);
    }
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t data_size) {
    if (ctx == NULL || data == NULL || data_size == 0 || data_size > 65536) {
        return 0;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->data = malloc(data_size);
    if (block->data == NULL) {
        return 0;
    }
    
    memcpy(block->data, data, data_size);
    block->block_size = data_size;
    block->compressed_block_size = data_size;
    block->block_offset = ctx->header.compressed_size;
    
    ctx->header.compressed_size += data_size;
    ctx->header.original_size += data_size;
    ctx->block_count++;
    
    return 1;
}

void simulate_compression(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->block_count == 0) return;
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        if (block->block_size > 10) {
            block->compressed_block_size = block->block_size - (block->block_size / 10);
            if (block->compressed_block_size > block->block_size) {
                block->compressed_block_size = block->block_size;
            }
        } else {
            block->compressed_block_size = block->block_size;
        }
    }
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Statistics:\n");
    printf("  Original size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("  Block count: %u\n", ctx->block_count);
    
    uint32_t total_compressed = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        total_compressed += ctx->blocks[i].compressed_block_size;
    }
    
    if (ctx->header.original_size > 0) {
        float ratio = (float)total_compressed / ctx->header.original_size * 100.0f;
        printf("  Compression ratio: %.1f%%\n", ratio);
    }
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("  Block %u: %u -> %u bytes\n", i, 
               ctx->blocks[i].block_size, 
               ctx->blocks[i].compressed_block_size);
    }
}

int main(void) {
    struct CompressionContext ctx;
    
    printf("Compression Stub Demo\n");
    printf("Enter number of data blocks to process (1-5): ");
    
    uint32_t block_count;
    if (scanf("%u", &block_count) != 1 || block_count < 1 || block_count > 5) {
        printf("Invalid input. Using default of 3 blocks.\n");
        block_count = 3;
    }
    
    while (getchar() != '\n');
    
    if (!init_compression_context(&ctx, block_count)) {
        printf("Failed to initialize compression context\n");
        return 1;
    }
    
    for (uint32_t i = 0; i < block_count; i++) {
        printf("Enter data for block %u (max 64 characters): ", i + 1);
        
        char input