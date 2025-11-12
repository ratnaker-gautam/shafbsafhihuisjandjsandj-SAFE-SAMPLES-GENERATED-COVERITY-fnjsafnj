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
    if (ctx == NULL) {
        return;
    }
    
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
    block->block_offset = ctx->header.original_size;
    
    ctx->header.original_size += data_size;
    ctx->header.compressed_size += data_size;
    ctx->block_count++;
    
    return 1;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->block_count == 0) {
        return 0;
    }
    
    uint32_t total_compressed = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        block->compressed_block_size = block->block_size - (block->block_size / 10);
        if (block->compressed_block_size < 1) {
            block->compressed_block_size = 1;
        }
        total_compressed += block->compressed_block_size;
    }
    
    ctx->header.compressed_size = total_compressed;
    return 1;
}

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Version: %u\n", ctx->header.version);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    if (ctx->header.original_size > 0) {
        printf("  Compression Ratio: %.2f%%\n", 
               (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    } else {
        printf("  Compression Ratio: 0.00%%\n");
    }
    
    printf("\nBlocks (%u total):\n", ctx->block_count);
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        printf("  Block %u: offset=%u, original=%u, compressed=%u\n",
               i, block->block_offset, block->block_size, block->compressed_block_size);
    }
}

int main(void) {
    struct CompressionContext ctx;
    
    if (!init_compression_context(&ctx, 5)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t sample_data1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t sample_data2[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE,