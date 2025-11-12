//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum[16];
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint8_t data[4096];
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
    if (header->original_size > UINT64_MAX / 2) return 0;
    if (header->compressed_size > UINT64_MAX / 2) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) return 0;
    
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    memset(ctx->header.checksum, 0, sizeof(ctx->header.checksum));
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    
    for (uint32_t i = 0; i < max_blocks; i++) {
        ctx->blocks[i].block_id = 0;
        ctx->blocks[i].offset = 0;
        ctx->blocks[i].size = 0;
        memset(ctx->blocks[i].data, 0, sizeof(ctx->blocks[i].data));
    }
    
    return 1;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 4096) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    
    uint32_t index = ctx->block_count;
    ctx->blocks[index].block_id = index + 1;
    ctx->blocks[index].offset = ctx->header.compressed_size;
    ctx->blocks[index].size = size;
    
    memcpy(ctx->blocks[index].data, data, size);
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += size;
    ctx->block_count++;
    
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint8_t checksum[16] = {0};
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        for (uint32_t j = 0; j < ctx->blocks[i].size; j++) {
            checksum[j % 16] ^= ctx->blocks[i].data[j];
        }
    }
    
    memcpy(ctx->header.checksum, checksum, sizeof(ctx->header.checksum));
}

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || !validate_header(&ctx->header)) {
        printf("Invalid compression context\n");
        return;
    }
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Version: %u\n", ctx->header.version);
    printf("  Original Size: %lu bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %lu bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Block Count: %u\n", ctx->block_count);
    
    printf("Checksum: ");
    for (int i = 0; i < 16; i++) {
        printf("%02X", ctx->header.checksum[i]);
    }
    printf("\n");
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("Block %u: offset=%u, size=%u\n", 
               ctx->blocks[i].block_id, 
               ctx->blocks[i].offset, 
               ctx->blocks[i].size);
    }
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    ctx->block_count = 0;
    ctx->max_blocks = 0;
}

int main(void) {
    struct CompressionContext ctx;
    
    if (!initialize_context(&ctx, 5)) {
        printf("Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t sample_data1[] = {0x48, 0x65, 0x