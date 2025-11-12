//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t data_offset;
    uint8_t compression_level;
    uint8_t reserved[3];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
    uint8_t *input_data;
    uint32_t input_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) return 0;
    
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 0;
    ctx->header.checksum = 0;
    ctx->header.flags = 0;
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    ctx->input_data = NULL;
    ctx->input_size = 0;
    
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t block_size, uint32_t data_offset, uint8_t compression_level) {
    if (ctx == NULL || ctx->blocks == NULL) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    if (block_size > 65536) return 0;
    if (compression_level > 9) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_size = block_size;
    block->data_offset = data_offset;
    block->compression_level = compression_level;
    memset(block->reserved, 0, sizeof(block->reserved));
    
    ctx->block_count++;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint8_t checksum = 0;
    checksum ^= (ctx->header.original_size >> 24) & 0xFF;
    checksum ^= (ctx->header.original_size >> 16) & 0xFF;
    checksum ^= (ctx->header.original_size >> 8) & 0xFF;
    checksum ^= ctx->header.original_size & 0xFF;
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        checksum ^= (ctx->blocks[i].block_size >> 24) & 0xFF;
        checksum ^= (ctx->blocks[i].block_size >> 16) & 0xFF;
        checksum ^= (ctx->blocks[i].block_size >> 8) & 0xFF;
        checksum ^= ctx->blocks[i].block_size & 0xFF;
    }
    
    ctx->header.checksum = checksum;
}

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || !validate_header(&ctx->header)) {
        printf("Invalid compression context\n");
        return;
    }
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Flags: 0x%02X\n", ctx->header.flags);
    printf("  Block Count: %u\n", ctx->block_count);
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("  Block %u: size=%u, offset=%u, level=%u\n", 
               i, ctx->blocks[i].block_size, ctx->blocks[i].data_offset, 
               ctx->blocks[i].compression_level);
    }
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    ctx->block_count = 0;
    ctx->max_blocks = 0;
    ctx->input_size