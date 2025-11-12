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
    uint32_t block_size;
    uint32_t checksum;
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
    if (header->magic != 0x43535455) return 0;
    if (header->version > 2) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) return 0;
    
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.compressed_size = 0;
    ctx->header.original_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    ctx->header.reserved = 0;
    
    ctx->blocks = malloc(sizeof(struct CompressionBlock) * max_blocks);
    if (ctx->blocks == NULL) return 0;
    
    for (uint32_t i = 0; i < max_blocks; i++) {
        ctx->blocks[i].block_size = 0;
        ctx->blocks[i].checksum = 0;
        ctx->blocks[i].data = NULL;
    }
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->blocks != NULL) {
        for (uint32_t i = 0; i < ctx->max_blocks; i++) {
            if (ctx->blocks[i].data != NULL) {
                free(ctx->blocks[i].data);
            }
        }
        free(ctx->blocks);
    }
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    
    uint32_t index = ctx->block_count;
    ctx->blocks[index].data = malloc(size);
    if (ctx->blocks[index].data == NULL) return 0;
    
    memcpy(ctx->blocks[index].data, data, size);
    ctx->blocks[index].block_size = size;
    
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        checksum = (checksum + data[i]) & 0xFFFF;
    }
    ctx->blocks[index].checksum = checksum;
    
    ctx->header.compressed_size += size;
    ctx->header.original_size += size;
    ctx->block_count++;
    
    return 1;
}

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || !validate_header(&ctx->header)) {
        printf("Invalid compression context\n");
        return;
    }
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Version: %u\n", ctx->header.version);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("  Original size: %u bytes\n", ctx->header.original_size);
    printf("  Block count: %u\n", ctx->block_count);
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        printf("  Block %u: size=%u, checksum=0x%04X\n", 
               i, ctx->blocks[i].block_size, ctx->blocks[i].checksum);
    }
}

int main(void) {
    struct CompressionContext ctx;
    
    printf("Initializing compression context...\n");
    if (!init_compression_context(&ctx, 5)) {
        printf("Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t sample_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
    uint8_t sample_data2[] = {0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
    uint8_t sample_data3[] = {0x54, 0x65, 0x73, 0x74, 0x20, 0x44, 0x61, 0x74, 0x61};
    
    printf("Adding compression blocks...\n");
    if (!add_compression_block(&ctx, sample_data1, sizeof(s