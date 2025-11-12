//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint32_t checksum;
    uint8_t algorithm;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint32_t compressed_size;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->version > 1) return 0;
    if (header->algorithm > 3) return 0;
    if (header->original_size > UINT64_MAX / 2) return 0;
    if (header->compressed_size > header->original_size) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) return 0;
    
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.checksum = 0;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    
    for (uint32_t i = 0; i < max_blocks; i++) {
        ctx->blocks[i].block_id = 0;
        ctx->blocks[i].offset = 0;
        ctx->blocks[i].size = 0;
        ctx->blocks[i].compressed_size = 0;
        ctx->blocks[i].data = NULL;
    }
    
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t block_id, 
                         const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    
    uint32_t idx = ctx->block_count;
    ctx->blocks[idx].block_id = block_id;
    ctx->blocks[idx].offset = ctx->header.original_size;
    ctx->blocks[idx].size = size;
    
    uint32_t compressed_size = size - (size / 4);
    if (compressed_size < size / 2) compressed_size = size / 2;
    
    ctx->blocks[idx].compressed_size = compressed_size;
    
    ctx->blocks[idx].data = malloc(size);
    if (ctx->blocks[idx].data == NULL) return 0;
    
    memcpy(ctx->blocks[idx].data, data, size);
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += compressed_size;
    ctx->block_count++;
    
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data != NULL) {
            for (uint32_t j = 0; j < ctx->blocks[i].size && j < 256; j++) {
                checksum = (checksum + ctx->blocks[i].data[j]) & 0xFFFFFFFF;
            }
        }
        checksum = (checksum + ctx->blocks[i].block_id) & 0xFFFFFFFF;
    }
    ctx->header.checksum = checksum;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || !validate_header(&ctx->header)) {
        printf("Invalid compression context\n");
        return;
    }
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Version: %u\n", ctx->header.version);
    printf("  Original Size: %lu bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %lu bytes\n", ctx->header.compressed_size);
    printf("  Compression Ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("  Checksum: 0x%08X\n", ctx->header.checksum);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Block Count: %u\n", ctx->block_count);
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("  Block %u: ID=%u, Offset=%u, Size=%u, Compressed=%u\n",
               i, ctx->blocks[i].block_id, ctx->blocks[i].offset,