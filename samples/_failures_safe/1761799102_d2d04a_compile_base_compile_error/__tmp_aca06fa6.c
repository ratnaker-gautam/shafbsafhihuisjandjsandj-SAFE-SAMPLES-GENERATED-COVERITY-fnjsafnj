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
    uint32_t offset;
    uint16_t length;
    uint16_t encoded_length;
    uint8_t data[256];
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
    if (header->original_size > 1048576) return 0;
    if (header->compressed_size > 1048576) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) return 0;
    
    ctx->blocks = malloc(sizeof(struct CompressionBlock) * max_blocks);
    if (ctx->blocks == NULL) return 0;
    
    ctx->max_blocks = max_blocks;
    ctx->block_count = 0;
    
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    ctx->header.flags = 0;
    
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint16_t length) {
    if (ctx == NULL || data == NULL || length == 0 || length > 256) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->offset = ctx->header.original_size;
    block->length = length;
    
    uint16_t encoded_len = 0;
    for (uint16_t i = 0; i < length; i++) {
        if (encoded_len < 256) {
            block->data[encoded_len] = data[i];
            encoded_len++;
        }
    }
    
    block->encoded_length = encoded_len;
    ctx->header.original_size += length;
    ctx->header.compressed_size += encoded_len;
    ctx->block_count++;
    
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint8_t sum = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        for (uint16_t j = 0; j < block->encoded_length; j++) {
            sum ^= block->data[j];
        }
    }
    ctx->header.checksum = sum;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL || !validate_header(&ctx->header)) return;
    
    printf("Compression Statistics:\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("Blocks processed: %u\n", ctx->block_count);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
}

int main(void) {
    struct CompressionContext ctx;
    
    if (!init_compression_context(&ctx, 10)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t test_data1[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42};
    uint8_t test_data2[] = {0x43, 0x43, 0x44, 0x44, 0x45, 0x45, 0x46, 0x46};
    uint8_t test_data3[] = {0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
    
    if (!add_compression_block(&ctx, test_data1, sizeof(test_data1))) {
        fprintf(stderr, "Failed to add compression block 1\n");
        cleanup_compression_context(&ctx);
        return 1;
    }
    
    if (!add_compression_block(&ctx, test_data