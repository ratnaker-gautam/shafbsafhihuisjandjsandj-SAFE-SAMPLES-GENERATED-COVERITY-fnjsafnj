//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    if (header->magic != 0x43535442) return 0;
    if (header->version > 2) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int init_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) return 0;
    
    ctx->header.magic = 0x43535442;
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

int add_block(struct CompressionContext *ctx, uint32_t size, const uint8_t *data) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    
    uint32_t idx = ctx->block_count;
    ctx->blocks[idx].block_id = idx + 1;
    ctx->blocks[idx].offset = ctx->header.compressed_size;
    ctx->blocks[idx].size = size;
    
    uint32_t compressed_size = (size * 3) / 4;
    if (compressed_size < 1) compressed_size = 1;
    ctx->blocks[idx].compressed_size = compressed_size;
    
    ctx->blocks[idx].data = malloc(compressed_size);
    if (ctx->blocks[idx].data == NULL) return 0;
    
    for (uint32_t i = 0; i < compressed_size && i < size; i++) {
        ctx->blocks[idx].data[i] = data[i] ^ 0xAA;
    }
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += compressed_size;
    ctx->block_count++;
    
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint32_t sum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        for (uint32_t j = 0; j < ctx->blocks[i].compressed_size; j++) {
            sum = (sum + ctx->blocks[i].data[j]) & 0xFFFFFFFF;
        }
    }
    ctx->header.checksum = sum;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data != NULL) {
            free(ctx->blocks[i].data);
        }
    }
    
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
    }
}

int main(void) {
    struct CompressionContext ctx;
    
    if (!init_context(&ctx, 5)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t sample_data1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t sample_data2[] = {0x10, 0x20, 0x30, 0x40, 0x50};
    uint8_t sample_data3[] = {0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA};
    
    if (!add_block(&ctx, sizeof(sample_data1), sample_data1)) {
        fprintf(stderr, "Failed to add block 1\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (!add_block(&ctx, sizeof(sample_data2), sample_data2)) {
        fprintf(stderr, "Failed to add block 2\n");
        cleanup_context(&ctx